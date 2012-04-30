//this is /brl/bpro/core/vil_pro/processes/vil_set_float_image_pixel_process.cxx
#include<bprb/bprb_func_process.h>
#include<brdb/brdb_value.h>

#include<bbas_pro/bbas_1d_array_float.h>

#include<vil/vil_image_view.h>
#include<vil/vil_image_view_base.h>

namespace vil_set_float_image_pixel_process_globals
{
    const unsigned int n_inputs_ = 4;
    const unsigned int n_outputs_ = 0;
}

bool vil_set_float_image_pixel_process_cons( bprb_func_process& pro )
{
    using namespace vil_set_float_image_pixel_process_globals;

    vcl_vector<vcl_string> input_types_(n_inputs_);

    unsigned i = 0;
    input_types_[i++] = "vil_image_view_base_sptr";
    input_types_[i++] = "unsigned";//i
    input_types_[i++] = "unsigned";//j
    input_types_[i++] = "bbas_1d_array_float_sptr";//pixel array (or scalar)

    if( !pro.set_input_types(input_types_) )
		return false;

    return true;
}

bool vil_set_float_image_pixel_process( bprb_func_process& pro )
{
    using namespace vil_set_float_image_pixel_process_globals;

    if (pro.n_inputs() != n_inputs_) {
        vcl_cout << "vil_set_float_image_pixel_process: The number of inputs should be " << n_inputs_ << vcl_endl;
        return false;
    }

    unsigned i = 0;
    vil_image_view_base_sptr 
        base_sptr = pro.get_input<vil_image_view_base_sptr>(i++);
    unsigned x    = pro.get_input<unsigned>(i++);
    unsigned y    = pro.get_input<unsigned>(i++);
    bbas_1d_array_float_sptr 
        pixel_array_sptr = pro.get_input<bbas_1d_array_float_sptr>(i++);

    if( x > base_sptr->ni() || y > base_sptr->nj() )
    {
        vcl_cerr << "-----ERROR-----" << vcl_endl
                 << "In vil_set_float_image_pixel_process" << vcl_endl
                 << "Target (" << x << ", "<< y << ") out of bounds." << vcl_endl;
        return false;
    }

    if( pixel_array_sptr->data_array.size() != base_sptr->nplanes() )
    {
        vcl_cerr << "-----ERROR-----" << vcl_endl
                 << "In vil_set_float_image_pixel_process" << vcl_endl
                 << "Pixel array and Image dimensions don't match." << vcl_endl;
        return false;
    }

    vil_image_view<float>* img_ptr = 
        dynamic_cast<vil_image_view<float>*>(base_sptr.as_pointer());

    if( !img_ptr )
    {
        vcl_cerr << "-----ERROR-----" << vcl_endl
                 << "In vil_set_float_image_pixel_process" << vcl_endl
                 << "Couldn't cast to vil_image_view<float>*" << vcl_endl;
        return false;
    }

    for( unsigned k = 0; k < pixel_array_sptr->data_array.size(); ++k )
        (*img_ptr)(x,y,k) = pixel_array_sptr->data_array[k];

    return true;
}