//this is brl/bpro/core/vil_pro/processes/vil_image_normalise_process
#include <bprb/bprb_func_process.h>
//:
// \file
// Scale and offset image to have zero mean and unit variance
#include<vil/vil_convert.h>
#include<vil/vil_image_resource.h>
#include<vil/vil_math.h>

namespace vil_image_normalise_process_globals
{
    const unsigned int n_inputs_ = 1;
    const unsigned int n_outputs_ = 0;
}

bool vil_image_normalise_process_cons(bprb_func_process& pro)
{
    using namespace vil_image_normalise_process_globals;

    vcl_vector<vcl_string> input_types_(n_inputs_);
    
    input_types_[0] = "vil_image_view_base_sptr";

    return pro.set_input_types(input_types_);
}

bool vil_image_normalise_process(bprb_func_process& pro)
{
    using namespace vil_image_normalise_process_globals;

    if (pro.n_inputs() < n_inputs_) {
        vcl_cout << "vil_set_float_image_pixel_process: "
                 << "The number of inputs should be " << n_inputs_ << vcl_endl;
        return false;
    }

    vil_image_view_base_sptr baseSptr =
        pro.get_input<vil_image_view_base_sptr>(0);

    //assume can convert to float
    vil_image_view<float> fview = *vil_convert_cast(float(), baseSptr);

    vil_math_normalise(fview);

    return true;
}
