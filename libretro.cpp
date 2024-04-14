// *****************************************************************************
    // include libretro headers
    #include "glsym/glsym.h"
    #include "libretro.h"
    
    // include Vircon32 headers
    #include "ConsoleLogic/V32Console.hpp"
    #include "VirconDefinitions/Constants.hpp"
    #include "VirconDefinitions/Enumerations.hpp"
    
    // include emulator headers
    #include "VideoOutput.hpp"
    #include "Globals.hpp"
    #include "Logging.hpp"
    
    // include C/C++ headers
    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
    #include <time.h>
    #include <sstream>
    
    // include the autogenerated embedded bios file
    #include <embedded/StandardBios.h>
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      DATA STRUCTURES DEFINING GAMEPADS AND PORTS
// =============================================================================


// the Vircon32 gamepad is defined taking as a base the basic SNES-like
// libretro pad and selects from it only the d-pad and the needed 7 buttons
struct retro_input_descriptor controller_descriptor[] =
{
    // gamepad when connected to port 1
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
    { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
    
    // gamepad when connected to port 2
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
    { 1, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
    
    // gamepad when connected to port 3
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
    { 2, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
    
    // gamepad when connected to port 4
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT,  "Left" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP,    "Up" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN,  "Down" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L" },
    { 3, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R" },
    
    // add a null termination to signal end of array
    { 0, 0, 0, 0, 0 }
};

// -----------------------------------------------------------------------------

// define the possible types of Vircon32 controllers
// (just one: Vircon32 only supports the standard gamepad)
const struct retro_controller_description controller_types[] =
{
    { "Vircon32 Gamepad", RETRO_DEVICE_JOYPAD }
};

// -----------------------------------------------------------------------------

// define all 4 Vircon32 controller ports (all are identical)
const struct retro_controller_info controller_ports[] =
{
    { controller_types, 1 },  // second parameter is number of types (i.e. array size)
    { controller_types, 1 },  // repeat for each of the 4 ports
    { controller_types, 1 },
    { controller_types, 1 },
    { nullptr, 0 }            // add a null termination to signal end of array
};


// =============================================================================
//      HANDLING FRAMESKIPPING
// =============================================================================


// internal configuration variable
bool enable_frameskip = false;

// we will store audio buffer status and use
// it to decide when we should skip a frame
bool audio_buffer_active = false;
bool audio_buffer_underrun_likely = false;
unsigned audio_buffer_occupancy = 0;

// -----------------------------------------------------------------------------

// callback to receive audio buffer status
static void retro_audio_buff_status_cb( bool active, unsigned occupancy, bool underrun_likely )
{
    audio_buffer_active = active;
    audio_buffer_occupancy = occupancy;
    audio_buffer_underrun_likely = underrun_likely;
}

// -----------------------------------------------------------------------------

// frameskip will need to be configured both initially
// and whenever its value is changed in the front-end
void configure_frameskip()
{
    struct retro_audio_buffer_status_callback buf_status_cb;
    buf_status_cb.callback = retro_audio_buff_status_cb;
    
    // with frameskip enabled, tell front-end we want to receive audio buffer status
    if( enable_frameskip )
    {
        // careful: automatic frameskipping needs audio buffer info;
        // however, some front-ends might not support this
        if( !environ_cb( RETRO_ENVIRONMENT_SET_AUDIO_BUFFER_STATUS_CALLBACK, &buf_status_cb ))
        {
            LOG( "Automatic frame skip has been disabled because frontend does not support audio buffer status monitoring." );
            audio_buffer_underrun_likely = false;
            return;
        }
    }
    
    // otherwise no need for that
    else environ_cb( RETRO_ENVIRONMENT_SET_AUDIO_BUFFER_STATUS_CALLBACK, nullptr );
}


// =============================================================================
//      HANDLING CORE-SPECIFIC OPTIONS
// =============================================================================


// configuration variables for this core
struct retro_variable config_variables[] =
{
    { "enable_frameskip", "Automatic frame skip; Disabled|Enabled" },
    { nullptr, nullptr }
};

// -----------------------------------------------------------------------------

static void update_config_variables()
{
    // use this to ask frontend for a variable value
    struct retro_variable variable_state =
    {
        "enable_frameskip", nullptr
    };
    
    if( environ_cb( RETRO_ENVIRONMENT_GET_VARIABLE, &variable_state ) && variable_state.value )
    {
        enable_frameskip = !strcmp( variable_state.value, "Enabled" );
        LOG( string("Automatic frame skip ") + (enable_frameskip? "enabled" : "disabled" ) );
        
        configure_frameskip();
    }
}


// =============================================================================
//      SETTING UP THE CORE ENVIRONMENT
// =============================================================================


void retro_set_controller_port_device( unsigned port, unsigned device )
{
    Console.SetGamepadConnection( port, (device == RETRO_DEVICE_JOYPAD) );
}

// -----------------------------------------------------------------------------

void retro_get_system_info( struct retro_system_info *info )
{
    memset( info, 0, sizeof( *info ) );
    info->library_name     = "Vircon32";
    info->library_version  = "2024.04.14";
    info->need_fullpath    = true;          // games can be too large to hold in memory
    info->valid_extensions = "v32|V32";     // target system may be case sensitive
}

// -----------------------------------------------------------------------------

void retro_get_system_av_info( struct retro_system_av_info *info )
{
    // video and audio frequencies
    info->timing.fps = 60;
    info->timing.sample_rate = 44100.0;
    
    // screen resolution is fixed 
    info->geometry.base_width  = V32::Constants::ScreenWidth;
    info->geometry.base_height = V32::Constants::ScreenHeight;
    info->geometry.max_width   = V32::Constants::ScreenWidth;
    info->geometry.max_height  = V32::Constants::ScreenHeight;

    // 0 means ratio = width/height
    info->geometry.aspect_ratio = 0;
}

// -----------------------------------------------------------------------------

void retro_set_environment( retro_environment_t cb )
{
    // capture the provided function
    environ_cb = cb;
    
    // we need to state that this is a core that casn be started
    // without a game! Otherwise the core will immediately unload
    bool no_rom = true;
    environ_cb( RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_rom );
    
    // define our controllers and controller ports
    environ_cb( RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, (void*)controller_descriptor );
    environ_cb( RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)controller_ports );
    
    // define this core's configuration variables
    environ_cb( RETRO_ENVIRONMENT_SET_VARIABLES, &config_variables );
    
    // fill callback function for logging
    struct retro_log_callback logging;
    
    if( cb( RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging ) )
      log_cb = logging.log;
}


// =============================================================================
//      MAIN EXECUTION FUNCTION
// =============================================================================


void retro_run()
{
    // if config variables have changed, update them
    bool variables_changed = false;
    
    if( environ_cb( RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &variables_changed ) && variables_changed )
      update_config_variables();
    
    // determine if this frame will be skipped
    bool skip_frame = enable_frameskip && audio_buffer_active && audio_buffer_underrun_likely;
    
    // when possible, run a full frame
    if( !skip_frame )
    {
        // read input for all connected gamepads
        input_poll_cb();
        
        for( int Port = 0; Port < V32::Constants::GamepadPorts; Port++ )
        {
            if( !Console.HasGamepad( Port ) )
              continue;
            
            // read all controls
            Console.SetGamepadControl( Port, V32::GamepadControls::Left,        input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT  ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::Right,       input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::Up,          input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP    ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::Down,        input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN  ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonStart, input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonA,     input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A     ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonB,     input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B     ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonX,     input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X     ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonY,     input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y     ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonL,     input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L     ) );
            Console.SetGamepadControl( Port, V32::GamepadControls::ButtonR,     input_state_cb( Port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R     ) );
        }
        
        // run the console
        if( !Console.IsPowerOn() )
          Console.SetPower( true );
        
        Video.BeginFrame();
        Console.RunNextFrame( false );
        
        // ensure that all queued quads are rendered
        Video.RenderQuadQueue();
        
        // send this frame's video signal to libretro
        video_cb( RETRO_HW_FRAME_BUFFER_VALID, V32::Constants::ScreenWidth, V32::Constants::ScreenHeight, 0 );        
        
        // send this frame's audio signal to libretro
        Console.GetFrameSoundOutput( AudioBuffer );
        audio_batch_cb( (const int16_t*)AudioBuffer.Samples, V32::Constants::SPUSamplesPerFrame );
    }
    
    // when a frame is skipped, only generate audio
    // to avoid a buffer underrun and prevent crackling
    else
    {
        // generate 1 frame's worth of audio
        Console.RunNextFrame( false );
        
        // send this frame's audio signal to libretro
        Console.GetFrameSoundOutput( AudioBuffer );
        audio_batch_cb( (const int16_t*)AudioBuffer.Samples, V32::Constants::SPUSamplesPerFrame );
    }
}


// =============================================================================
//      AUXILIARY FUNCTIONS FOR FILE AND PATH HANDLING
// =============================================================================


bool FileExists( const string& FilePath )
{
    ifstream TestedFile;
    TestedFile.open( FilePath );

    return (bool)TestedFile;
}

// -----------------------------------------------------------------------------

// determine the path of the memory card corresponding to a
// given game path, taking into account the system save directory
string GetMemoryCardPath( const string& CartridgePath )
{
    // step 1: determine the system's save path
    const char *SaveDirectory = nullptr;
	environ_cb( RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY, &SaveDirectory );
    
    // step 2: unify path separators to forward slashes
    string SaveDirectoryUnified = SaveDirectory;
    for( auto& c: SaveDirectoryUnified )
      if( c == '\\' )
        c = '/';
    
    string CartridgePathUnified = CartridgePath;
    for( auto& c: CartridgePathUnified )
      if( c == '\\' )
        c = '/';
    
    // step 2: isolate the file name
    size_t SlashPosition = CartridgePathUnified.rfind( '/' );
    string FileName;
    
    if( SlashPosition == string::npos )
      FileName = CartridgePathUnified;
    else
      FileName = CartridgePathUnified.substr( SlashPosition + 1, CartridgePathUnified.size() - 1 );
    
    // step 3: replace or add the file extension
    size_t DotPosition = FileName.rfind( '.' );
    
    if( DotPosition == string::npos )
      return SaveDirectoryUnified + "/" + FileName + ".memc";
    else
      return SaveDirectoryUnified + "/" + FileName.substr( 0, DotPosition ) + ".memc";
}


// =============================================================================
//      ROUTINE FOR LOADING VIRCON32 BIOS
// =============================================================================


void LoadEmbeddedBios()
{
    V32::Callbacks::LogLine( "Loading embedded bios" );
    
    // save the bios bytes into a stringstream
    std::stringstream InputData;
    InputData.write( (const char *)embedded_StandardBios, sizeof( embedded_StandardBios ) );
    
    // now load the bios from the stream
    Console.LoadBiosData( InputData );
}

// -----------------------------------------------------------------------------

void LoadVircon32Bios()
{
    // determine system directory
    const char *SystemDirPath = 0;
    environ_cb( RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &SystemDirPath );
    
    // if there is an alternate bios file load it first
    string BiosFilePath = SystemDirPath + string("/Vircon32Bios.v32");
    
    if( FileExists( BiosFilePath ) )
      Console.LoadBiosFile( BiosFilePath );
    
    // otherwise load the default bios embedded in the core
    else LoadEmbeddedBios();
}


// =============================================================================
//      HANDLING CONTEXT FOR CORE AND OPENGL
// =============================================================================


void context_reset()
{
    LOG( "Received signal: Reset context" );
    rglgen_resolve_symbols( hw_render.get_proc_address );
    
    // initialize video output
    Video.InitRendering();
    
    // set console's video callbacks
    V32::Callbacks::ClearScreen = CallbackFunctions::ClearScreen;
    V32::Callbacks::DrawQuad = CallbackFunctions::DrawQuad;
    V32::Callbacks::SetMultiplyColor = CallbackFunctions::SetMultiplyColor;
    V32::Callbacks::SetBlendingMode = CallbackFunctions::SetBlendingMode;
    V32::Callbacks::SelectTexture = CallbackFunctions::SelectTexture;
    V32::Callbacks::LoadTexture = CallbackFunctions::LoadTexture;
    V32::Callbacks::UnloadCartridgeTextures = CallbackFunctions::UnloadCartridgeTextures;
    V32::Callbacks::UnloadBiosTexture = CallbackFunctions::UnloadBiosTexture;
    
    // set console's log callbacks
    V32::Callbacks::LogLine = CallbackFunctions::LogLine;
    V32::Callbacks::ThrowException = CallbackFunctions::ThrowException;
    
    // obtain current time
    time_t CreationTime;
    time( &CreationTime );
    struct tm* CreationTimeInfo = localtime( &CreationTime );
    
    // set console date and time
    // (Careful! C gives year counting from 1900)
    Console.SetCurrentDate( CreationTimeInfo->tm_year + 1900, CreationTimeInfo->tm_yday );
    Console.SetCurrentTime( CreationTimeInfo->tm_hour, CreationTimeInfo->tm_min, CreationTimeInfo->tm_sec );
    
    // operations that load files may throw on failure
    try
    {
        // load the console bios
        LoadVircon32Bios();
        
        // load a cartridge if it was specified
        if( !LoadedCartridgePath.empty() )
        {
            Console.LoadCartridge( LoadedCartridgePath );
            
            // also load the corresponding memory card if existing
            if( FileExists( LoadedMemoryCardPath ) )
              Console.LoadMemoryCard( LoadedMemoryCardPath );
            
            // otherwise create an empty card and load it
            else
            {
                Console.CreateMemoryCard( LoadedMemoryCardPath );
                Console.LoadMemoryCard( LoadedMemoryCardPath );
            }
        }
    }
    catch( const exception& e )
    {
        LOG( "ERROR: " + string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------

void context_destroy()
{
    LOG( "Received signal: Destroy context" );
    Console.UnloadCartridge();
    Console.UnloadBios();
    Video.Destroy();
}

// -----------------------------------------------------------------------------

bool retro_init_hw_context()
{
    LOG( "Received signal: Init HW context" );
    
    // HW context info dependent on the device
    #if defined(HAVE_OPENGLES2)
      hw_render.context_type = RETRO_HW_CONTEXT_OPENGLES2;
    #elif defined(HAVE_OPENGLES3)
      hw_render.context_type = RETRO_HW_CONTEXT_OPENGLES3;
    #else
      hw_render.context_type = RETRO_HW_CONTEXT_OPENGL_CORE;
      hw_render.version_major = 3;
      hw_render.version_minor = 0;
    #endif
    
    // fixed HW context details
    hw_render.context_reset = context_reset;
    hw_render.context_destroy = context_destroy;
    hw_render.depth = false;
    hw_render.stencil = false;
    hw_render.bottom_left_origin = true;
    
    if( !environ_cb( RETRO_ENVIRONMENT_SET_HW_RENDER, &hw_render ) )
       return false;
    
    return true;
}


// =============================================================================
//      BASIC QUERIES FROM THE FRONT-END
// =============================================================================


unsigned retro_api_version()
{
   return RETRO_API_VERSION;
}

// -----------------------------------------------------------------------------

unsigned retro_get_region()
{
    // we are not tied to any physical region,
    // but return NTSC since we use 60 fps
    return RETRO_REGION_NTSC;
}


// =============================================================================
//      OBTAINING CALLBACK FUNCTIONS FROM THE FRONT-END
// =============================================================================


void retro_set_audio_sample(retro_audio_sample_t cb)
{
    audio_cb = cb;
}

// -----------------------------------------------------------------------------

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
    audio_batch_cb = cb;
}

// -----------------------------------------------------------------------------

void retro_set_input_poll(retro_input_poll_t cb)
{
    input_poll_cb = cb;
}

// -----------------------------------------------------------------------------

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

// -----------------------------------------------------------------------------

void retro_set_video_refresh(retro_video_refresh_t cb)
{
    video_cb = cb;
}


// =============================================================================
//      GENERAL CORE OPERATION
// =============================================================================


void retro_init()
{
    LOG( "Received signal: Init" );
    // (nothing to do for this core)
}

// -----------------------------------------------------------------------------

void retro_deinit()
{
    LOG( "Received signal: Deinit" );
    // (nothing to do for this core)
}

// -----------------------------------------------------------------------------

void retro_reset()
{
    LOG( "Received signal: Reset" );
    Console.Reset();
}

// -----------------------------------------------------------------------------

bool retro_load_game( const struct retro_game_info *info )
{
    LOG( "Received signal: Load game" );
    
    // before loading ensure that config variables are updated
    update_config_variables();
    
    // for OpenGL we will normally want 32bpp color format
    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    
    if( !environ_cb( RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt ) )
    {
        LOG( "ERROR: XRGB8888 is not supported." );
        return false;
    }
    
    // initialize our context
    if( !retro_init_hw_context() )
    {
        LOG( "ERROR: HW Context could not be initialized" );
        return false;
    }
    
    // case 1: core loaded with a game
    if( info && info->path )
    {
        LOG( string("Core loaded with game: ") + info->path );
        LoadedCartridgePath = info->path;
        
        // build a path for the game's memory card
        LoadedMemoryCardPath = GetMemoryCardPath( LoadedCartridgePath );
    }
    
    // case 2: core loaded with no game
    // (the console will run the BIOS)
    else
    {
        LOG( "Core loaded with no game" );
        LoadedCartridgePath = "";
    }
    
    return true;
}

// -----------------------------------------------------------------------------

void retro_unload_game()
{
    LOG( "Received signal: Unload game" );
    
    Console.UnloadCartridge();
    Console.UnloadMemoryCard();
}

// -----------------------------------------------------------------------------

bool retro_load_game_special( unsigned type, const struct retro_game_info *info, size_t num )
{
    // not implemented: we only use regular load game
    (void)type;
    (void)info;
    (void)num;
    return false;
}


// =============================================================================
//      HANDLING INTERNAL MEMORY (NOT SUPPORTED)
// =============================================================================


void *retro_get_memory_data( unsigned id )
{
    // not implemented
    (void)id;
    return nullptr;
}

// -----------------------------------------------------------------------------

size_t retro_get_memory_size( unsigned id )
{
    // not implemented
    return 0;
}


// =============================================================================
//      HANDLING SAVE-STATES (NOT SUPPORTED)
// =============================================================================


size_t retro_serialize_size()
{
    // not implemented
    return 0;
}

// -----------------------------------------------------------------------------

bool retro_serialize(void *data, size_t size )
{
    // not implemented
    (void)data;
    (void)size;
    return false;
}

// -----------------------------------------------------------------------------

bool retro_unserialize( const void *data, size_t size )
{
    // not implemented
    (void)data;
    (void)size;
    return false;
}


// =============================================================================
//      HANDLING CHEAT SUPPORT (NOT SUPPORTED)
// =============================================================================


void retro_cheat_reset()
{
    // not implemented
}

// -----------------------------------------------------------------------------

void retro_cheat_set( unsigned index, bool enabled, const char *code )
{
    // not implemented
    (void)index;
    (void)enabled;
    (void)code;
}
