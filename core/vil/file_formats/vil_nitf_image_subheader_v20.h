// $Source$

//================ GE Aerospace NITF support libraries =================
//
// Description:	This file defines the NITF headers for NITF v1.1.
//
//  $Revision$ 
//  $Date$
//  $Author$
//
// For a given header 'head', head->Read(file) is called when the
// appropriate place in the file is reached, and it will return a
// StatusCode indicating success or failure.
//
// Written by:       Burt Smith
// Date:             August 26, 1992
//
//=====================lkbjfcbtdddhtargbskmtaps=======================

#ifndef	_vil_nitf_imagesubheader_v20_h
#define _vil_nitf_imagesubheader_v20_h

#include "vil_nitf_header.h"
#include "vil_nitf_image_subheader.h"
#include "vil_nitf_util.h"

#include <vcl_sstream.h>

//====================================================================
// The Image Sub-Header
//====================================================================

// MPP 4/15/2001
// Added support for PIAIMC extension
#define PIAIMC_CEL      362     // PIAIMC Controlled Extension Length

class vil_nitf_image_subheader_v20 : public vil_nitf_image_subheader
{
    public:
         vil_nitf_image_subheader_v20();
         vil_nitf_image_subheader_v20 (const vil_nitf_image_subheader&);
         vil_nitf_image_subheader_v20 (const vil_nitf_image_subheader_v20&);
         virtual ~vil_nitf_image_subheader_v20();

         virtual bool IsCompressed() const ;

         virtual int GetCloudCoverPercentage() const ;
  
         virtual StatusCode Read (vil_stream*);
         virtual StatusCode Write (vil_stream*);

         virtual int GetHeaderLength();

      // Need to generalize for NITFv2.1
      // Move these up to the vil_nitf_image_subheader base class
      //char* IREP;  
      //char* ICAT;

         void Init();                   // Clears entire header.

    private:
	 StatusCode extract_user_defined_record_extensions() ;
         StatusCode extract_rpc00x_extension();
         StatusCode extract_ichipb_extension();
         StatusCode extract_piaimc_extension();
         StatusCode extract_stdid_extension();
         void encode_rpc00a_extension (vcl_ostringstream& rpc_buf);
         void encode_ichipb_extension (vcl_ostringstream& ichipb_buf);
         void encode_piaimc_extension (vcl_ostringstream& piaimc_buf);

         void init();                   // Initializes header.  MUST only
                                        // be called from constructor.
};

#endif
