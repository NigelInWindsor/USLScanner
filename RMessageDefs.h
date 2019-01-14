#ifndef RMESSAGE_DEFS_6234873247562340826323423478562342
#define RMESSAGE_DEFS_6234873247562340826323423478562342

//To be sent when the CRemote data is updated.
#define RMESSAGE_WM_REMOTEUPDATE (WM_USER+300)
#define REMOTE_WM_UPDATECONNECTIONSTATUS (WM_USER+301)

//Normalize
#define REMOTE_NORMALIZE (WM_USER+302)
#define REMOTE_PEAKOPPOSITE (WM_USER+303)
#define REMOTE_WATERPATHLEFT (WM_USER+304)
#define REMOTE_WATERPATHRIGHT (WM_USER+305)
#define REMOTE_MOVETOLEFT (WM_USER+306)
#define REMOTE_MOVETORIGHT (WM_USER+307)
#define REMOTE_GETNORMALIZEDATA (WM_USER+308)
#define REMOTE_HANDLER (WM_USER+309)
#define REMOTE_GETNORMALIZESETTINGS (WM_USER+319)
#define REMOTE_GETPEAKORTHOGONALSETTINGS (WM_USER+320)
#define REMOTE_GETWATERPATHLEFTSETTINGS (WM_USER+321)
#define REMOTE_GETWATERPATHRIGHTSETTINGS (WM_USER+322)


//Profile
#define REMOTE_ADD_COORD (WM_USER+310)
#define REMOTE_DELETE_COORD (WM_USER+311)
#define REMOTE_NEW_LINE (WM_USER+312)
#define REMOTE_HANDLER_PROFILE (WM_USER+313)
#define REMOTE_OPEN3DVIEWEDITDLG (WM_USER+323)

//Remote
#define REMOTE_RMESSAGE_SEND (WM_USER+314)
#define REMOTE_RMESSAGE_RECEIVE (WM_USER+315)
#define REMOTE_START_SERVER (WM_USER+316)

//Ultrasonics
#define REMOTE_HANDLER_ULTRASONICS (WM_USER+317)
#define REMOTE_UPDATE_ALL_CONTROLS (WM_USER+318)

//FLSC1) Laser rangefinder
#define REMOTE_HANDLER_FLSC10 (WM_USER+324)
#define REMOTE_FLSC10_MEAS_REC (WM_USER+325)
#define REMOTE_FLSC10_ERR (WM_USER+326)
#define REMOTE_FLSC10_MISC (WM_USER+327)


//Egismos Laser Rangefinder
#define REMOTE_HANDLER_EGISMOSRF (WM_USER+328)
#define REMOTE_EGISMOSRF_MEAS_REC (WM_USER+329)
#define REMOTE_EGISMOSRF_ERR (WM_USER+330)
#define REMOTE_EGISMOSRF_MISC (WM_USER+331)

//Remote page 
#define REMOTE_PAGE_UPDATE_CONTROLS (WM_USER+332)

//Next = +333

#endif //#ifndef RMESSAGE_DEFS_6234873247562340826323423478562342
