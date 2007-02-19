#include <vcl_cstdlib.h>
#include <vcl_string.h>

#include <vsl/vsl_indent.h>
#include <mcal/mcal_pca.h>
#include <mbl/mbl_matrix_products.h>
#include <vcl_cassert.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vcl_cmath.h>
#include <vnl/vnl_math.h>
#include <vcl_vector.h>
#include <vsl/vsl_binary_io.h>
#include <mbl/mbl_parse_block.h>
#include <mbl/mbl_read_props.h>
#include <vul/vul_string.h>
#include <mbl/mbl_exception.h>

//=======================================================================
// Constructors
//=======================================================================

mcal_pca::mcal_pca()
{
  min_modes_ = 0;
  max_modes_ = 9999;
  var_prop_ = 0.98;

  max_d_in_memory_ = 1.0e8;
  use_chunks_=false;
}

//=======================================================================
// Destructor
//=======================================================================

mcal_pca::~mcal_pca()
{
}


//: Return the number of modes to retain
unsigned mcal_pca::choose_n_modes(const vnl_vector<double>& evals)
{
  if (evals.size()==0) return 0;
  const double* v_data = evals.begin();
  unsigned n=evals.size();

  if (var_prop_==0.0) return 0;

  // Find last positive eigenvalue
  unsigned i=0;
  while (i<n && v_data[i]>0) i++;
  n = i;

  if (n==0) return 0;

  if ((var_prop_>=1.0) || (var_prop_<=0.0))
  {
    if ((n>max_modes_) && (max_modes_>=0))
      return max_modes_;
    else
      return n;
  }

  // Compute total variance
  double total_var=0.0;
  for (i=0;i<n;++i) total_var += v_data[i];
  if (total_var==0.0) return 0;

  double target_var = total_var * var_prop_;

  // Find number of modes required to achieve target_var;
  double var=0.0;
  unsigned n_modes=0;
  while (var<target_var && n_modes<n)
    { var+=v_data[n_modes]; n_modes++;}

  if (n_modes>max_modes_) return max_modes_;
  if (n_modes<min_modes_ && min_modes_<=n) return min_modes_;
  return n_modes;
}

//: Compute eigenvectors assuming fewer dimensions than samples
void mcal_pca::build_evecs_nd_smaller(mbl_data_wrapper<vnl_vector<double> >& data,
                                const vnl_vector<double>& mean,
                                vnl_matrix<double>& evecs,
                                vnl_vector<double>& evals)
{
  data.reset();
  int n_samples = data.size();
  int n_dims = data.current().size();

  vnl_matrix<double> S(n_dims,n_dims);
  double **s_data = S.data_array();
  S.fill(0);

  vnl_vector<double> dv;
  for (int k=0;k<n_samples;++k)
  {
    dv = data.current();
    dv-=mean;
    const double* v_data = dv.begin();
    for (int i=0;i<n_dims;++i)
    {
      double *s_row = s_data[i];
      double v_i = v_data[i];
      for (int j=0;j<n_dims;++j)
        s_row[j] += v_i*v_data[j];
    }

    data.next();
  }

  S/=n_samples;

  // Compute eigenvectors and values of S
  vnl_matrix<double> EVecs(n_dims,n_dims);
  vnl_vector<double> EVals(n_dims);
  vnl_symmetric_eigensystem_compute(S,EVecs,EVals);

  // Note: EVecs and EVals are ordered with the smallest first
  evals = EVals;
  evals.flip();
  unsigned n_modes = choose_n_modes(evals);

  // Extract the relevant n_modes from EVecs and EVals
  evecs.set_size(n_dims,n_modes);
  evals.set_size(n_modes);
  double **EV_data = EVecs.data_array();
  double **ev_data = evecs.data_array();

  for (unsigned i=0;i<n_dims;++i)
  {
    // Flip each row
    double *EV = EV_data[i]-1;
    double *ev = ev_data[i];
    for (unsigned j=0;j<n_modes;++j) ev[j]=EV[n_dims-j];
  }
  for (unsigned j=0;j<n_modes;++j)
    evals[j]=EVals[n_dims-1-j];
}

//: Compute eigenvectors assuming fewer samples than dimensions
void mcal_pca::build_evecs_ns_smaller(mbl_data_wrapper<vnl_vector<double> >& data,
                                const vnl_vector<double>& mean,
                                vnl_matrix<double>& evecs,
                                vnl_vector<double>& evals)
{
  data.reset();
  // Principle components are generated by computing eigenvectors
  // of a n_dims square matrix, DDt
  int n_samples = data.size();
  int n_dims = data.current().size();

  double n_total = n_samples * n_dims;
  int n_chunks = 1 + int(2*n_total/max_d_in_memory_);
  double chunk_size = double(n_samples)/n_chunks;

  vnl_matrix<double> DDt(n_samples,n_samples);
  vnl_matrix<double> D;
  vnl_vector<double> dv;

  static bool message_given_before = false;
  if (!use_chunks_ && n_chunks>1 && !message_given_before)
  {
    message_given_before=true;
    vcl_cerr<<"\n"
              "WARNING - mcal_pca:  The size of the matrix is\n"
              "  possibly too large for the memory. If the build fails, try\n"
              "  setting set_use_chunks to true in the mcal_pca.\n"<<vcl_endl;
  }


  // If all chunks can be managed in one pass
  if (!use_chunks_ || n_chunks<=2 )
  {
    // Read data into a single matrix
    D.set_size(n_samples,n_dims);
    for (int i=0;i<n_samples;i++)
    {
      dv = data.current();
      dv -= mean;
      D.set_row(i,dv);
      data.next();
    }

    mbl_matrix_product_a_bt(DDt,D,D);
  }

  else
  {
    vcl_cout<<vsl_indent()<<"mcal_pca:  Constructing DD_t in "<<n_chunks<<" passes."<<vcl_endl;

    // Set up indices for chunks
    vcl_vector<int> start(n_chunks),end(n_chunks);
    start[0] = 0;
    end[0] = vnl_math_rnd(chunk_size)-1;
    for (int i=1;i<n_chunks;i++) 
    {
      start[i] = end[i-1]+1;
      end[i] = vnl_math_rnd((i+1)*chunk_size) -1;
    }
    end[n_chunks-1] = n_samples-1;

    vnl_matrix<double> D1,D2,DDt1;
    for (int i=0;i<n_chunks;i++)
    {
      int n = 1+end[i]-start[i];
      D1.set_size(n,n_dims);
      data.set_index(start[i]);
      for (int j=0;j<n;++j)
      {
        dv=data.current();
        dv-=mean;
        D1.set_row(j,dv);
        data.next();
      }

      mbl_matrix_product_a_bt(DDt1,D1,D1);
      fillDDt(DDt,DDt1,start[i],end[i],start[i],end[i]);

      for (int j=i+1;j<n_chunks;j++)
      {
        //data.chunk(D2,start[j],end[j]);
        int n = 1+end[j]-start[j];
        D2.set_size(n,n_dims);
        data.set_index(start[j]);
        for (int k=0;k<n;++k)
        {
          dv=data.current();
          dv-=mean;
          D2.set_row(k,dv);
          data.next();
        }
        //subtractMean(D2);

        mbl_matrix_product_a_bt(DDt1,D1,D2);
        fillDDt(DDt,DDt1,start[i],end[i],start[j],end[j]);
      }
    }
  }

  DDt/=n_samples;

  // Compute eigenvectors
  vnl_matrix<double> EVecs(n_samples,n_samples);
  vnl_vector<double> EVals;
  evals.set_size(n_samples);
  vnl_symmetric_eigensystem_compute(DDt,EVecs,EVals);

  // Arrange that values/vectors ordered with largest first
  EVals.flip();
  EVecs.fliplr();

  unsigned n_modes = 0;
  if (n_samples>=2)
    n_modes = choose_n_modes(EVals);

  if (n_modes==0)
  {
    evals.set_size(0);
    evecs.set_size(0,0);
    return;
  }

  // Retain larges n_modes eigenvalues
  evals.set_size(n_modes);
  for (unsigned i=0;i<n_modes;++i) evals[i]=EVals[i];

  // Now compute the principal components from PC = D.transpose() * EVecs
  evecs.set_size(n_dims,n_modes);
  double **EV_data = EVecs.data_array();
  double **ev_data = evecs.data_array();
  evecs.fill(0);

  double **D_data = D.data_array();
  if (!use_chunks_ || n_chunks<=2)
  {
    for (int i=0;i<n_samples;++i)
    {
      double* dv_data = D_data[i];
      double* EV_row = EV_data[i];
      for (int j=0;j<n_modes;++j)
      {
        double EV = EV_row[j];
        for (int k=0;k<n_dims;++k)
          ev_data[k][j] += dv_data[k]*EV;
      }
    }
  }
  else
  {
    vnl_vector<double> dv;
    data.reset();
    for (int i=0;i<n_samples;++i)
    {
      dv = data.current();
      dv -= mean;
      double* dv_data = dv.begin();
      double* EV_row = EV_data[i];
      for (int j=0;j<n_modes;++j)
      {
        double EV = EV_row[j];
        for (int k=0;k<n_dims;++k)
          ev_data[k][j] += dv_data[k]*EV;
      }

      data.next();
    }
  }

  // Normalise each column
  vnl_vector<double> col_sum(n_modes);
  col_sum.fill(0);
  double* cs = col_sum.begin();
  for (int k=0;k<n_dims;k++)
  {
    const double* row = ev_data[k];
    for (int j=0;j<n_modes;++j)
      cs[j]+=row[j]*row[j];
  }
  for (int j=0;j<n_modes;++j) cs[j]=1.0/vcl_sqrt(cs[j]);
  for (int k=0;k<n_dims;k++)
  {
    double* row = ev_data[k];
    for (int j=0;j<n_modes;++j)
      row[j]*=cs[j];
  }
}

//: Support function for chunks
void mcal_pca::fillDDt(vnl_matrix<double>& DDt, const vnl_matrix<double>& A, 
           int rlo, int rhi, int clo, int chi)
{
  double **DDt_data = DDt.data_array();
  const double *const*A_data = A.data_array();
  
  for (int r=rlo;r<=rhi;++r)
  {
    double *DDt_row = DDt_data[r];
    const double* A_row = A_data[r-rlo]-clo;
    for (int c=clo;c<=chi;c++)
    {
      DDt_row[c] = A_row[c];
      DDt_data[c][r] = A_row[c];    // Transpose
    }
  }
}

void mcal_pca::set_mode_choice(unsigned min, unsigned max,
                    double var_proportion)
{
  min_modes_ = min;
  max_modes_ = max;
  var_prop_ = var_proportion;
}

//: Set the choice for the minimum number of model
void mcal_pca::set_min_modes( const unsigned min )
{
  min_modes_ = min;
}

  
//: Current lower limit on number of parameters
unsigned mcal_pca::min_modes() const
{
  return min_modes_;
}

void mcal_pca::set_max_modes(unsigned max)
{
  max_modes_ = max;
}

  
//: Current upper limit on number of parameters
unsigned mcal_pca::max_modes() const
{
  return max_modes_;
}
  
//: Define proportion of data variance to explain
void mcal_pca::set_var_prop(double v)
{
  var_prop_ = v;
}
  
//: Current proportion of data variance to explain
double mcal_pca::var_prop() const
{  
  return var_prop_;
}

void mcal_pca::set_max_d_in_memory(double max_n)
{
  max_d_in_memory_ = max_n;
}

//: Set whether we may build in chunks if required
void mcal_pca::set_use_chunks(bool chunks)
{
  use_chunks_=chunks;
}

//: Compute modes of the supplied data relative to the supplied mean
//  Model is x = mean + modes*b,  where b is a vector of weights on each mode.
//  mode_var[i] gives the variance of the data projected onto that mode.
void mcal_pca::build_about_mean(mbl_data_wrapper<vnl_vector<double> >& data,
                                const vnl_vector<double>& mean,
                                vnl_matrix<double>& modes,
                                vnl_vector<double>& mode_var)
{
  if (data.size()==0)
  {
    vcl_cerr<<"mcal_pca::build_about_mean() No samples supplied."<<vcl_endl;
    abort();
  }

  data.reset();

  if (data.current().size()==0)
  {
    vcl_cerr<<"mcal_pca::build_about_mean() ";
    vcl_cerr<<"Warning: Samples claim to have zero dimensions."<<vcl_endl;
    vcl_cerr<<"Constructing empty model."<<vcl_endl;

    modes.set_size(0,0);
    mode_var.set_size(0);
    return;
  }

  // Choose suitable strategy for computing principal components
  if (data.current().size()<data.size())
    build_evecs_nd_smaller(data,mean,modes,mode_var);
  else
    build_evecs_ns_smaller(data,mean,modes,mode_var);

}



//=======================================================================
// Method: is_a
//=======================================================================

vcl_string  mcal_pca::is_a() const
{ 
  return vcl_string("mcal_pca");
}

//=======================================================================
// Method: version_no
//=======================================================================

short mcal_pca::version_no() const 
{ 
  return 1; 
}

//=======================================================================
// Method: clone
//=======================================================================

mcal_component_analyzer* mcal_pca::clone() const
{
  return new mcal_pca(*this);
}

//=======================================================================
// Method: print
//=======================================================================

void mcal_pca::print_summary(vcl_ostream& os) const
{
  os<<vcl_endl;
  os<<vsl_indent()<<"min_modes: "<<min_modes_<<vcl_endl;
  os<<vsl_indent()<<"max_modes: "<<max_modes_<<vcl_endl;
  os<<vsl_indent()<<"var_prop: "<<var_prop_<<vcl_endl;
  os<<vsl_indent()<<"max_d_in_memory: "<<max_d_in_memory_<<vcl_endl;
  os<<vsl_indent()<<"use_chunks: "<<use_chunks_<<vcl_endl;
}

//=======================================================================
// Method: save
//=======================================================================

void mcal_pca::b_write(vsl_b_ostream& bfs) const
{
  vsl_b_write(bfs,version_no());
  vsl_b_write(bfs,min_modes_);
  vsl_b_write(bfs,max_modes_);
  vsl_b_write(bfs,var_prop_);
  vsl_b_write(bfs,max_d_in_memory_);
  vsl_b_write(bfs,use_chunks_);
}

//=======================================================================
// Method: load
//=======================================================================

void mcal_pca::b_read(vsl_b_istream& bfs)
{
  short version;
  vsl_b_read(bfs,version);
  switch (version)
  {
    case (1):
      vsl_b_read(bfs,min_modes_);
      vsl_b_read(bfs,max_modes_);
      vsl_b_read(bfs,var_prop_);
      vsl_b_read(bfs,max_d_in_memory_);
      vsl_b_read(bfs,use_chunks_);
      break;
    default:
      vcl_cerr << "mcal_pca::b_read() ";
      vcl_cerr << "Unexpected version number " << version << vcl_endl;
      abort();
  }
}

//=======================================================================
//: Read initialisation settings from a stream.
// Parameters:
// \verbatim
// {
//   min_modes: 0 max_modes: 99 prop_modes: 0.99
//   // Maximum number of doubles to store in memory at once
//   max_d_in_memory: 1e8
//   // Indicate how to build from large amounts of data
//   use_chunks: false
// }
// \endverbatim
// \throw mbl_exception_parse_error if the parse fails.
void mcal_pca::config_from_stream(vcl_istream & is)
{
  vcl_string s = mbl_parse_block(is);

  vcl_istringstream ss(s);
  mbl_read_props_type props = mbl_read_props_ws(ss);


  {
    int m_min =0,m_max=999;
    double m_props=0.05;
    if (!props["min_modes"].empty())
    {
      m_min=vul_string_atoi(props["min_modes"]);
    }
    props.erase("min_modes");

    if (!props["max_modes"].empty())
    {
      m_max=vul_string_atoi(props["max_modes"]);
    }
    props.erase("max_modes");

    if (!props["var_prop"].empty())
    {
      m_props=vul_string_atof(props["var_prop"]);
    }
    props.erase("var_prop");

    set_mode_choice(m_min,m_max,m_props);
  }

  if (!props["max_d_in_memory"].empty())
  {
    max_d_in_memory_=vul_string_atof(props["max_d_in_memory"]);
  }
  props.erase("max_d_in_memory");


  {
    if (!props["use_chunks"].empty())
    {
      use_chunks_ = vul_string_to_bool(props["use_chunks"]);
      props.erase("use_chunks");
    }
    else
      use_chunks_=false;

    props.erase("use_chunks");
  }


  try
  {
      mbl_read_props_look_for_unused_props(
          "mcal_pca::config_from_stream", props);
  }
  catch(mbl_exception_unused_props &e)
  {
    throw mbl_exception_parse_error(e.what());
  }

}



