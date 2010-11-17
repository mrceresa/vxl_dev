#ifndef boxm2_data_txx
#define boxm2_data_txx
#include <boxm2/boxm2_data.h>

template <boxm2_data_type T>
boxm2_data<T>::boxm2_data(unsigned char * data_buffer, 
                       vcl_size_t length, 
                       boxm2_block_id id)
                       :boxm2_data_base(data_buffer,length,id)
{
    data_array_=new boxm2_array_1d<datatype>(buffer_length_/sizeof(datatype),reinterpret_cast<datatype*>(data_buffer));
}

template <boxm2_data_type T>
boxm2_data<T>::~boxm2_data()
{
    delete data_array_;
}

#undef BOXM2_DATA_INSTANTIATE
#define BOXM2_DATA_INSTANTIATE(T) \
template class boxm2_data<T>; 


#endif //boxm2_data_txx