#include "tr_local.h"
#include "tr_layer.h"
#include "proxy_main.h"

// D3D headers
#include "../d3d11/d3d_driver.h"

// GL headers
#include "gl_common.h"
#include "gl_image.h"

// @pjb: this is just here to deliberately fuck the build if driver is used in here
#define driver #driver_disallowed

static graphicsApiLayer_t openglDriver;

void PROXY_Shutdown( void )
{
    openglDriver.Shutdown();
}

void PROXY_UnbindResources( void )
{
    openglDriver.UnbindResources();
}

size_t PROXY_LastError( void )
{
    //@pjb: todo: decide which one of these takes preference
    size_t glError = openglDriver.LastError();
    
    return glError;
}

void PROXY_ReadPixels( int x, int y, int width, int height, imageFormat_t requestedFmt, void* dest )
{
    //@pjb: todo: decide which one of these takes preference
    openglDriver.ReadPixels( x, y, width, height, requestedFmt, dest );
}

void PROXY_CreateImage( const image_t* image, const byte *pic, qboolean isLightmap )
{
    openglDriver.CreateImage( image, pic, isLightmap );
}

void PROXY_DeleteImage( const image_t* image )
{
    openglDriver.DeleteImage( image );
}

imageFormat_t PROXY_GetImageFormat( const image_t* image )
{
    // todo: CALL the D3D one and discard the results
    // Just do the GL one
    return openglDriver.GetImageFormat( image );
}

void PROXY_SetGamma( unsigned char red[256], unsigned char green[256], unsigned char blue[256] )
{
    openglDriver.SetGamma( red, green, blue );
    // @pjb: todo
}

int PROXY_SumOfUsedImages( void )
{
    return openglDriver.GetFrameImageMemoryUsage(); 
    // @pjb: todo
}

void PROXY_GfxInfo( void )
{
	ri.Printf( PRINT_ALL, "----- OpenGL -----\n" );
    openglDriver.GraphicsInfo( );
    // @pjb: todo
	ri.Printf( PRINT_ALL, "----- PROXY -----\n" );
    ri.Printf( PRINT_ALL, "Using proxied driver: all commands issued to OpenGL and D3D11.\n" );
}

void PROXY_Clear( unsigned long bits, const float* clearCol, unsigned long stencil, float depth )
{
    openglDriver.Clear( bits, clearCol, stencil, depth );
    // @pjb: todo
}

void PROXY_SetProjection( const float* projMatrix )
{
    openglDriver.SetProjection( projMatrix );
}

void PROXY_SetViewport( int left, int top, int width, int height )
{
    openglDriver.SetViewport( left, top, width, height );
}

void PROXY_Flush( void )
{
    openglDriver.Flush();
}

void PROXY_SetState( unsigned long stateMask )
{
    openglDriver.SetState( stateMask );
}

void PROXY_DriverInit( graphicsApiLayer_t* layer )
{
    layer->Shutdown = PROXY_Shutdown;
    layer->UnbindResources = PROXY_UnbindResources;
    layer->LastError = PROXY_LastError;
    layer->ReadPixels = PROXY_ReadPixels;
    layer->CreateImage = PROXY_CreateImage;
    layer->DeleteImage = PROXY_DeleteImage;
    layer->GetImageFormat = PROXY_GetImageFormat;
    layer->SetGamma = PROXY_SetGamma;
    layer->GetFrameImageMemoryUsage = PROXY_SumOfUsedImages;
    layer->GraphicsInfo = PROXY_GfxInfo;
    layer->Clear = PROXY_Clear;
    layer->SetProjection = PROXY_SetProjection;
    layer->SetViewport = PROXY_SetViewport;
    layer->Flush = PROXY_Flush;
    layer->SetState = PROXY_SetState;

    // If using the proxy driver we cannot use fullscreen
    Cvar_Set( "r_fullscreen", "0" );

    // Proxy OpenGL
    GLRB_DriverInit( &openglDriver );

    // todo: proxy D3D11


    R_ValidateGraphicsLayer( &openglDriver );
    // tod: R_ValidateGraphicsLayer( &d3dDriver );
    
    // Copy the resource strings to the vgConfig
    Q_strncpyz( vdConfig.renderer_string, "PROXY DRIVER", sizeof( vdConfig.renderer_string ) );
    Q_strncpyz( vdConfig.vendor_string, "@pjb", sizeof( vdConfig.vendor_string ) );
    Q_strncpyz( vdConfig.version_string, "1.0", sizeof( vdConfig.version_string ) );
    
    // todo: arbitrate conflicting vdConfig settings

}

