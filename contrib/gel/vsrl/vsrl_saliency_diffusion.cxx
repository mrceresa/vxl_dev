#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vcl_cstddef.h>
#include <vcl_cstdio.h>
#include <vcl_cstdlib.h>

#include <vsrl/vsrl_saliency_diffusion.h>

vsrl_saliency_diffusion::vsrl_saliency_diffusion(vsrl_dense_matcher *matcher):
  vsrl_diffusion(matcher)
{
  _saliency=0;
  _disparaty_matrix->fill(0.0);
  
}

vsrl_saliency_diffusion::~vsrl_saliency_diffusion()
{

}


void vsrl_saliency_diffusion::set_initial_disparaty(vsrl_diffusion *step_diff)
{
  // we want to set the initial dispararty

  int x,y;
  double dd;
  

  for(x=0;x<_width;x++){
    for(y=0;y<_height;y++){
      dd = step_diff->get_disparaty(x,y);
      (*_disparaty_matrix)(x,y)=dd;
    }
  }
}

void vsrl_saliency_diffusion::set_saliency(vsrl_token_saliency *saliency)
{
  // set the object that knows whether or not a pixel is salient

  _saliency = saliency;
}


void vsrl_saliency_diffusion::difuse_disparaty(int num_iter)
{

  // OK we want to use diffusion so that disparaty
  // will propogate from stong regions of salienct
  // to regions of low disparaty

  vcl_cout << "Starting to diffuse" << vcl_endl;

  vnl_matrix<double> mat1= (*_disparaty_matrix);
  vnl_matrix<double> mat2= mat1;
  
  vnl_matrix<double> *mstar1 = (&mat1);
  vnl_matrix<double> *mstar2 = (&mat2);
  vnl_matrix<double> *hold;
  

  
  // OK start to diffuse 

    
  int x,y;
  double N,sum;
  int dif_num;
  int i,j;
  double val;
 
   for(dif_num=0;dif_num<num_iter;dif_num++){
     vcl_cout << "Saliency Diffusion Iteration " << dif_num << vcl_endl;
     vcl_cout << " disparaty for pixel 700,353 is " << (*mstar1)(700,353) << vcl_endl; 

     // make an image of the current disparaty

     write_image("/projects/IUP2/peter_tu/tmp/fig",dif_num,mstar1);

     // start to difuse 
     
     int dif_range = 5; // The range that we are will to diffuse over 

     for(x=dif_range;x<_width -dif_range;x++){
      
       for(y=dif_range;y<_height - dif_range;y++){
	 
	 // only process non salient pixels 
	 
	 if(!(_saliency->get_saliency(x,y))){
	   
	   
	   // get the average value of mat1(x,y)'s neighborhood
	   N=0;
	   sum=0;
	   
	   
	   for(i=x-dif_range;i<x+dif_range+1;i++){
	     for(j=y-dif_range;j<y+dif_range+1;j++){
	       val = (*mstar1)(i,j);
	       if(val!=1000){
		 // this pixel has some information to offer so 
		 // diffuse naturally 
		 
		 sum=sum+val;
		 N++;
	       }
	     }
	   }
	   if(N){
	     // we have some information
	     sum=sum/N;
	     // store the difused value
	     (*mstar2)(x,y)=sum;
	   }
	 }
       }
     }
    

     // OK Swap mstar1 and mstar2
     hold=mstar1;
     mstar1=mstar2;
     mstar2=hold;
    
   }

   // get rid of the pockets of unitialized data 
   for(x=0;x<_width;x++){
     for(y=0;y<_height;y++){
       val = (*mstar1)(x,y);
       if(val==1000){
	 (*mstar1)(x,y)=0;
       }
     }
   }
   
   


   // copy the new results 
   (*_disparaty_matrix)=(*mstar1);
   
   vcl_cout << "Finished the diffusion " << vcl_endl;

}

void vsrl_saliency_diffusion::execute(int num_iter)
{

  
  // execute the disparaty stuff
  
  // modify the intitial disparaty_matrix using the results of 
  // the saliency object 

  if(_saliency){
    consider_saliency();
  }
  
  // run a diffusion algorithm

   difuse_disparaty(num_iter);
}

void vsrl_saliency_diffusion::consider_saliency()
{
  // each pixel is marked if it is not salient

  int x,y;
  
  for(x=0;x<_width;x++){
    for(y=0;y<_height;y++){
      if(!(_saliency->get_saliency(x,y))){
	// this pixel is not salient so mark it with 1000 disparaty
	
	(*_disparaty_matrix)(x,y)=1000; // this value will not be used for diffusion 
      }
    }
  }
}
	
      
 
  

 


