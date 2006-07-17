// This is mul/m23d/tests/test_arc.cxx
#include <testlib/testlib_test.h>
#include <vcl_iostream.h>
#include <m23d/m23d_make_ortho_projection.h>
#include <m23d/m23d_ortho_flexible_builder.h>
#include <vcl_cmath.h>
#include <vnl/vnl_random.h>
#include <m23d/m23d_rotation_matrix.h>

void test_projection_matrix(const vnl_matrix<double>& P)
{
  unsigned ns=P.rows()/2;
  unsigned m=P.cols()/3-1;

  bool test_ok=true;
  vcl_cout<<"Testing projection matrix."<<vcl_endl;
  for (unsigned k=0;k<=m;++k)
  {
    // Test orthogonality of rows of each projection matrix
    for (unsigned i=0;i<ns;++i)
    {
      vnl_matrix<double> Pik = P.extract(2,3,2*i,3*k);
      vnl_matrix<double> PPt=Pik*Pik.transpose();
      if (vcl_fabs(PPt(0,1))>1e-5)
      {
        vcl_cout<<"View "<<i<<" mode "<<k<<" ) rows not orthogonal. "<<PPt(0,1)<<vcl_endl;
        test_ok=false;
      }
      if (vcl_fabs(PPt(0,0)-PPt(1,1))>1e-5)
      {
        vcl_cout<<"View "<<i<<" mode "<<k<<" ) rows don't scale equally. "<<PPt(0,0)-PPt(1,1)<<vcl_endl;
        test_ok=false;
      }
    }
  }
  if (test_ok)
    vcl_cout<<"Projection matrix passed the tests."<<vcl_endl;

}

void test_ortho_flexible_builder_on_rigid()
{
  vcl_cout<<"==== test m23d_ortho_flexible_builder ====="<<vcl_endl;

  vnl_random r(35813);
  unsigned ns = 20;
  vnl_matrix<double> P=m23d_make_ortho_projection(r,ns,0,true,true);

  // Generate cube test data
  vnl_matrix<double> C(3,8);
  unsigned c=0;
  for (int i=-1;i<=1;i+=2)
    for (int j=-1;j<=1;j+=2)
      for (int k=-1;k<=1;k+=2,++c)
      {
        C(0,c)=i*vcl_sqrt(1.0/3.0);
        C(1,c)=j*vcl_sqrt(1.0/3.0);
        C(2,c)=k*vcl_sqrt(1.0/3.0);
      }

  vnl_matrix<double> D=P*C;

  m23d_ortho_flexible_builder builder;
  builder.reconstruct(D,0);

  TEST("Size of P1",(builder.projections().rows()==2*ns
                      && builder.projections().cols()==3),true);
  TEST("Size of P3D",(builder.shape_3d().rows()==3
                      && builder.shape_3d().cols()==8),true);
  vnl_matrix<double> P1  = builder.projections();
  vnl_matrix<double> P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-D).rms(),0,1e-6);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,1e-6);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,1e-6);

  vnl_matrix<double> pure_P0(2,3);
  pure_P0(0,0)=1; pure_P0(0,1)=0; pure_P0(0,2)=0;
  pure_P0(1,0)=0; pure_P0(1,1)=1; pure_P0(1,2)=0;

  vcl_cout<<"First projection:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3)<<vcl_endl;
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,1e-6);


  vcl_cout<<"Test scaled cube"<<vcl_endl;
  C*=2.5;
  D=P*C;
  builder.reconstruct(D,0);
  P1  = builder.projections();
  P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-D).rms(),0,1e-6);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,1e-6);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,1e-6);
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,1e-6);

  vcl_cout<<"Add some gaussian noise to the 2D observations."<<vcl_endl;
  for (unsigned i=0;i<D.rows();++i)
    for (unsigned j=0;j<D.cols();++j)
      D(i,j)+=0.1*r.normal64();

  builder.reconstruct(D,0);
  P1  = builder.projections();
  P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-D).rms(),0,0.1);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,0.1);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,0.1);
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,0.05);

  vnl_matrix<double> P_0 = P1.extract(2,3);
  vcl_cout<<"P0:"<<vcl_endl<<P_0<<vcl_endl;
  vcl_cout<<"P0.P0'="<<vcl_endl<<P_0*P_0.transpose()<<vcl_endl;
  vnl_matrix<double> P_1 = P1.extract(2,3,2,0);
  vcl_cout<<"P0:"<<vcl_endl<<P_1<<vcl_endl;
  vcl_cout<<"P0.P0'="<<vcl_endl<<P_1*P_1.transpose()<<vcl_endl;

  vcl_cout<<"=== Test refinement ==="<<vcl_endl;
  builder.refine();
  P1  = builder.projections();
  P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-D).rms(),0,0.1);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,0.1);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,0.1);
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,0.05);


//  vcl_cout<<"Recovered 3D points: "<<vcl_endl<<P3D<<vcl_endl;
//  vcl_cout<<"True 3D points: "<<vcl_endl<<C<<vcl_endl;
}

  // Generate cube test data, with one (symmetric) mode
vnl_matrix<double> make_cube_model()
{
  vnl_matrix<double> C(6,14,0.0);
  // First 3 rows are a cube
  unsigned c=0;
  for (int i=-1;i<=1;i+=2)
    for (int j=-1;j<=1;j+=2)
      for (int k=-1;k<=1;k+=2,++c)
      {
        C(0,c)=i*vcl_sqrt(1.0/3.0);
        C(1,c)=j*vcl_sqrt(1.0/3.0);
        C(2,c)=k*vcl_sqrt(1.0/3.0);
      }
  // Next three modes are points moving along x,y,z
  for (unsigned i=0;i<3;++i)
  {
    C(3+i,8+2*i)=-vcl_sqrt(0.5);
    C(3+i,9+2*i)= vcl_sqrt(0.5);
  }

  return C;
}

void test_ortho_flexible_builder_on_flexible()
{
  vcl_cout<<"==== test m23d_ortho_flexible_builder ====="<<vcl_endl;

  vnl_random r(35813);
  unsigned ns = 20;
  unsigned n_modes = 1;
  vnl_matrix<double> P=m23d_make_ortho_projection(r,ns,n_modes,true,true);

  vnl_matrix<double> C=make_cube_model();

  vnl_matrix<double> D=P*C;

  m23d_ortho_flexible_builder builder;
  builder.reconstruct(D,1);

  TEST("Size of P1",(builder.projections().rows()==2*ns
                      && builder.projections().cols()==6),true);
  TEST("Size of P3D",(builder.shape_3d().rows()==6
                      && builder.shape_3d().cols()==C.cols()),true);
  vnl_matrix<double> P1  = builder.projections();
  vnl_matrix<double> P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-builder.centred_views()).rms(),0,1e-6);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,1e-6);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,1e-6);

  TEST_NEAR("RMS error on first mode of P1",(P-P1).extract(2*ns,3).rms(),0,1e-6);


  vnl_matrix<double> pure_P0(2,3);
  pure_P0(0,0)=1; pure_P0(0,1)=0; pure_P0(0,2)=0;
  pure_P0(1,0)=0; pure_P0(1,1)=1; pure_P0(1,2)=0;

  vcl_cout<<"Projection 0, mode 0:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3)<<vcl_endl;
  TEST_NEAR("Projection 0, mode 0 is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,0.02);
  vcl_cout<<"Projection 0, mode 1:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,0,3)<<vcl_endl;

  vcl_cout<<"Projection 1, mode 0:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,2,0)<<vcl_endl;
  vcl_cout<<"Projection 1, mode 1:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,2,3)<<vcl_endl;

  vcl_cout<<"Projection 4, mode 0:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,8,0)<<vcl_endl;
  vcl_cout<<"Projection 4, mode 1:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,8,3)<<vcl_endl;
  vcl_cout<<"Projection 5, mode 0:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,10,0)<<vcl_endl;
  vcl_cout<<"Projection 5, mode 1:"<<vcl_endl;
  vcl_cout<<P1.extract(2,3,10,3)<<vcl_endl;
  vcl_cout<<"True Projection 5, mode 0:"<<vcl_endl;
  vcl_cout<<P.extract(2,3,10,0)<<vcl_endl;
  vcl_cout<<"True Projection 5, mode 1:"<<vcl_endl;
  vcl_cout<<P.extract(2,3,10,3)<<vcl_endl;

  vcl_cout<<"=== Test compute_correction ==="<<vcl_endl;
  vnl_matrix<double> G;
  builder.compute_correction(P,G);
  vcl_cout<<"G for pure P"<<vcl_endl<<G<<vcl_endl;
  vnl_matrix<double> PG=P*G;
  vcl_cout<<"PG:"<<vcl_endl<<PG.extract(6,6)<<vcl_endl;

  vcl_cout<<"Apply a rotation to columns of P"<<vcl_endl;
  vnl_matrix<double> P2(P);
  vnl_matrix<double> R2=m23d_rotation_matrix(0.1,0.2,0.3);
  vcl_cout<<"Rotation applied: "<<vcl_endl<<R2<<vcl_endl;
  for (unsigned i=0;i<=n_modes;++i)
    P2.update(P.extract(2*ns,3,0,3*i)*R2,0,3*i);
  builder.compute_correction(P2,G);
  vcl_cout<<"G for rotated P"<<vcl_endl<<G<<vcl_endl;
  test_projection_matrix(P2*G);

  vcl_cout<<"Apply a rotation and scaling to columns of P"<<vcl_endl;
  for (unsigned i=0;i<=n_modes;++i)
    P2.update(P.extract(2*ns,3,0,3*i)*R2*(3+i),0,3*i);
  builder.compute_correction(P2,G);
  vcl_cout<<"G for scaled and rotated P"<<vcl_endl<<G<<vcl_endl;

  test_projection_matrix(P2);
  test_projection_matrix(P2*G);

  vcl_cout<<"=== Apply a random affine projection ==="<<vcl_endl;
  vnl_matrix<double> G0(G);
  for (unsigned i=0;i<G0.rows();++i)
    for (unsigned j=0;j<G0.cols();++j)  G0(i,j)=r.drand64(-1,1);
  vnl_matrix<double> P3=P*G0;
  builder.compute_correction(P3,G);
  test_projection_matrix(P3*G);
  vcl_cout<<"G0*G="<<vcl_endl<<G0*G<<vcl_endl;


/*
  vcl_cout<<"Test scaled cube"<<vcl_endl;
  C*=2.5;
  D=P*C;
  builder.reconstruct(D,1);
  P1  = builder.projections();
  P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-builder.centred_views()).rms(),0,1e-6);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,1e-6);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,1e-6);
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,1e-6);

  vcl_cout<<"Add some gaussian noise to the 2D observations."<<vcl_endl;
  for (unsigned i=0;i<D.rows();++i)
    for (unsigned j=0;j<D.cols();++j)
      D(i,j)+=0.1*r.normal64();

  builder.reconstruct(D,1);
  P1  = builder.projections();
  P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-builder.centred_views()).rms(),0,0.1);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,0.1);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,0.1);
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,0.05);

  vnl_matrix<double> P_0 = P1.extract(2,3);
  vcl_cout<<"P0:"<<vcl_endl<<P_0<<vcl_endl;
  vcl_cout<<"P0.P0'="<<vcl_endl<<P_0*P_0.transpose()<<vcl_endl;
  vnl_matrix<double> P_1 = P1.extract(2,3,2,0);
  vcl_cout<<"P0:"<<vcl_endl<<P_1<<vcl_endl;
  vcl_cout<<"P0.P0'="<<vcl_endl<<P_1*P_1.transpose()<<vcl_endl;

  vcl_cout<<"=== Test refinement ==="<<vcl_endl;
  builder.refine();
  P1  = builder.projections();
  P3D = builder.shape_3d();
  TEST_NEAR("RMS error on projection",(P1*P3D-D).rms(),0,0.1);
  TEST_NEAR("RMS error on P1",(P-P1).rms(),0,0.1);
  TEST_NEAR("RMS error on P3D",(P3D-C).rms(),0,0.1);
  TEST_NEAR("First projection is identity",
            (P1.extract(2,3)-pure_P0).rms(),0,0.05);

*/
// vcl_cout<<"Recovered 3D points: "<<vcl_endl<<P3D<<vcl_endl;
//  vcl_cout<<"True 3D points: "<<vcl_endl<<C<<vcl_endl;
}

void test_ortho_flexible_builder()
{
  test_ortho_flexible_builder_on_rigid();
//  test_ortho_flexible_builder_on_flexible();
}


TESTMAIN(test_ortho_flexible_builder);
