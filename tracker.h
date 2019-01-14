/* Generated code.  Do not modify. */

#ifndef __TRACKER_H__
#define __TRACKER_H__

typedef unsigned short* FAROLPTSTR;

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif

#ifdef _EXPORTING
#define CLASS_DECLSPEC    _declspec(dllexport)
#else
#define CLASS_DECLSPEC    _declspec(dllimport)
#define JNICALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

//void CLASS_DECLSPEC trackerDeleteString(WCHAR *msg);
void CLASS_DECLSPEC trackerDeleteString( FAROLPTSTR ptr);
bool CLASS_DECLSPEC trackerSetInstallDir( TCHAR	*dir );

class CLASS_DECLSPEC SMXException
{
private:
    TCHAR    msg[200];

public:
    SMXException( TCHAR    exceptionMsg[] );
    virtual ~SMXException();

    virtual void getMsg( TCHAR    exceptionMsg[],
                 int     maxLen           );
};

class CLASS_DECLSPEC TrkDrvException : public SMXException
{
private:
    TCHAR    msg[200];

public:
    TrkDrvException( TCHAR    exceptionMsg[] );
    virtual ~TrkDrvException();

    void getMsg( TCHAR    exceptionMsg[],
                 int     maxLen           );
};

class CLASS_DECLSPEC BaseDriverObject
{
public:
    BaseDriverObject();

    virtual ~BaseDriverObject();

};

class CLASS_DECLSPEC DriverStringPointer : public BaseDriverObject
{
public:
    DriverStringPointer(FAROLPTSTR stringDataArg);

    virtual ~DriverStringPointer();

    FAROLPTSTR getStringData();
private:
    FAROLPTSTR	stringData;
};

class CLASS_DECLSPEC SMXObjectArray
{
private:
    BaseDriverObject** elementBuffer;
    int   elementCount;

public:
    SMXObjectArray( int  numElements );

    virtual ~SMXObjectArray();

    virtual int getNumElements();

    virtual void setElement(int index, BaseDriverObject* element);

    virtual LPVOID getElement(int index);
};

class CLASS_DECLSPEC TrkDrvObjectArray : public SMXObjectArray
{
private:
    BaseDriverObject** elementBuffer;
    int   elementCount;

public:
    TrkDrvObjectArray( int  numElements );

    virtual ~TrkDrvObjectArray();

    int getNumElements();

    void setElement(int index, BaseDriverObject* element);

    LPVOID getElement(int index);
};

//***********************************************
class CLASS_DECLSPEC TrackerException
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    TrackerException(  );

    TrackerException( FAROLPTSTR param0 );

    explicit TrackerException( LPVOID param0 );

    virtual ~TrackerException();

    FAROLPTSTR getText(  );

    int getCode(  );

    static TrackerException * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC AbortedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    AbortedException(  );

    explicit AbortedException( LPVOID param0 );

    virtual ~AbortedException();

public:
};

//***********************************************
class CLASS_DECLSPEC AlreadyConnectedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    AlreadyConnectedException(  );

    explicit AlreadyConnectedException( LPVOID param0 );

    virtual ~AlreadyConnectedException();

public:
};

//***********************************************
class CLASS_DECLSPEC ApplicationFailedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    ApplicationFailedException(  );

    ApplicationFailedException( FAROLPTSTR param0 );

    explicit ApplicationFailedException( LPVOID param0 );

    virtual ~ApplicationFailedException();

public:
};

//***********************************************
class CLASS_DECLSPEC AsyncAlreadyStartedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    AsyncAlreadyStartedException(  );

    explicit AsyncAlreadyStartedException( LPVOID param0 );

    virtual ~AsyncAlreadyStartedException();

public:
};

//***********************************************
class CLASS_DECLSPEC BadCommandException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    BadCommandException(  );

    BadCommandException( FAROLPTSTR param0 );

    explicit BadCommandException( LPVOID param0 );

    virtual ~BadCommandException();

public:
};

//***********************************************
class CLASS_DECLSPEC ClassLoadException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    ClassLoadException(  );

    explicit ClassLoadException( LPVOID param0 );

    virtual ~ClassLoadException();

public:
};

//***********************************************
class CLASS_DECLSPEC CommunicationException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    CommunicationException(  );

    CommunicationException( FAROLPTSTR param0 );

    explicit CommunicationException( LPVOID param0 );

    virtual ~CommunicationException();

public:
};

//***********************************************
class CLASS_DECLSPEC ConnectFailedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    ConnectFailedException(  );

    ConnectFailedException( FAROLPTSTR param0 );

    explicit ConnectFailedException( LPVOID param0 );

    virtual ~ConnectFailedException();

public:
};

//***********************************************
class CLASS_DECLSPEC DataFormatException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    DataFormatException(  );

    explicit DataFormatException( LPVOID param0 );

    virtual ~DataFormatException();

public:
};

//***********************************************
class CLASS_DECLSPEC DeviceLockedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    DeviceLockedException(  );

    DeviceLockedException( FAROLPTSTR param0 );

    explicit DeviceLockedException( LPVOID param0 );

    virtual ~DeviceLockedException();

public:
};

//***********************************************
class CLASS_DECLSPEC DisconnectFailedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    DisconnectFailedException(  );

    explicit DisconnectFailedException( LPVOID param0 );

    virtual ~DisconnectFailedException();

public:
};

//***********************************************
class CLASS_DECLSPEC DistanceSetException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    DistanceSetException(  );

    DistanceSetException( FAROLPTSTR param0 );

    explicit DistanceSetException( LPVOID param0 );

    virtual ~DistanceSetException();

public:
};

//***********************************************
class CLASS_DECLSPEC DoubleFaultException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    DoubleFaultException(  );

    DoubleFaultException( FAROLPTSTR param0 );

    explicit DoubleFaultException( LPVOID param0 );

    virtual ~DoubleFaultException();

public:
};

//***********************************************
class CLASS_DECLSPEC FileAccessException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    FileAccessException(  );

    FileAccessException( FAROLPTSTR param0 );

    explicit FileAccessException( LPVOID param0 );

    virtual ~FileAccessException();

public:
};

//***********************************************
class CLASS_DECLSPEC InterfaceBusyException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    InterfaceBusyException(  );

    InterfaceBusyException( FAROLPTSTR param0 );

    explicit InterfaceBusyException( LPVOID param0 );

    virtual ~InterfaceBusyException();

public:
};

//***********************************************
class CLASS_DECLSPEC InternalDeviceFailureException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    InternalDeviceFailureException(  );

    InternalDeviceFailureException( FAROLPTSTR param0 );

    explicit InternalDeviceFailureException( LPVOID param0 );

    virtual ~InternalDeviceFailureException();

public:
};

//***********************************************
class CLASS_DECLSPEC InternalTrackerException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    InternalTrackerException(  );

    InternalTrackerException( FAROLPTSTR param0 );

    explicit InternalTrackerException( LPVOID param0 );

    virtual ~InternalTrackerException();

public:
};

//***********************************************
class CLASS_DECLSPEC LaserStateException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    LaserStateException(  );

    LaserStateException( FAROLPTSTR param0 );

    explicit LaserStateException( LPVOID param0 );

    virtual ~LaserStateException();

public:
};

//***********************************************
class CLASS_DECLSPEC MeasureTypeException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    MeasureTypeException(  );

    explicit MeasureTypeException( LPVOID param0 );

    virtual ~MeasureTypeException();

public:
};

//***********************************************
class CLASS_DECLSPEC MotorStateException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    MotorStateException(  );

    MotorStateException( FAROLPTSTR param0 );

    explicit MotorStateException( LPVOID param0 );

    virtual ~MotorStateException();

public:
};

//***********************************************
class CLASS_DECLSPEC NoDataAvailableException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    NoDataAvailableException(  );

    explicit NoDataAvailableException( LPVOID param0 );

    virtual ~NoDataAvailableException();

public:
};

//***********************************************
class CLASS_DECLSPEC NoResponseException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    NoResponseException(  );

    explicit NoResponseException( LPVOID param0 );

    virtual ~NoResponseException();

public:
};

//***********************************************
class CLASS_DECLSPEC NoTargetException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    NoTargetException(  );

    NoTargetException( FAROLPTSTR param0 );

    explicit NoTargetException( LPVOID param0 );

    virtual ~NoTargetException();

public:
};

//***********************************************
class CLASS_DECLSPEC NotConnectedException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    NotConnectedException(  );

    explicit NotConnectedException( LPVOID param0 );

    virtual ~NotConnectedException();

public:
};

//***********************************************
class CLASS_DECLSPEC PermissionException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    PermissionException(  );

    PermissionException( FAROLPTSTR param0 );

    explicit PermissionException( LPVOID param0 );

    virtual ~PermissionException();

public:
};

//***********************************************
class CLASS_DECLSPEC RequestException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    RequestException(  );

    RequestException( FAROLPTSTR param0 );

    explicit RequestException( LPVOID param0 );

    virtual ~RequestException();

public:
};

//***********************************************
class CLASS_DECLSPEC ShutterStateException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    ShutterStateException(  );

    ShutterStateException( FAROLPTSTR param0 );

    explicit ShutterStateException( LPVOID param0 );

    virtual ~ShutterStateException();

public:
};

//***********************************************
class CLASS_DECLSPEC TrackingStateException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    TrackingStateException(  );

    TrackingStateException( FAROLPTSTR param0 );

    explicit TrackingStateException( LPVOID param0 );

    virtual ~TrackingStateException();

public:
};

//***********************************************
class CLASS_DECLSPEC UnsupportedFeatureException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    UnsupportedFeatureException(  );

    UnsupportedFeatureException( FAROLPTSTR param0 );

    explicit UnsupportedFeatureException( LPVOID param0 );

    virtual ~UnsupportedFeatureException();

public:
};

//***********************************************
class CLASS_DECLSPEC UserEntryException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    UserEntryException(  );

    UserEntryException( FAROLPTSTR param0 );

    explicit UserEntryException( LPVOID param0 );

    virtual ~UserEntryException();

public:
};

//***********************************************
class CLASS_DECLSPEC UserPromptException: public TrackerException
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:
    virtual void throwThisException();
public:

    UserPromptException(  );

    UserPromptException( FAROLPTSTR param0 );

    explicit UserPromptException( LPVOID param0 );

    virtual ~UserPromptException();

public:
};

//***********************************************
class CLASS_DECLSPEC SMXObject: public BaseDriverObject

{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    SMXObject(  );

    explicit SMXObject( LPVOID param0 );

    virtual ~SMXObject();

    static SMXObject * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC Boolean: public SMXObject
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    Boolean( FAROLPTSTR param0 );

    Boolean( bool param0 );

    explicit Boolean( LPVOID param0 );

    virtual ~Boolean();

    int hashCode(  );

    int compareTo( Boolean * param0 );

    int compareTo( SMXObject * param0 );

    bool getBoolean( FAROLPTSTR param0 );

    bool equals( SMXObject * param0 );

    FAROLPTSTR toString( bool param0 );

    FAROLPTSTR toString(  );

    Boolean * valueOf( FAROLPTSTR param0 );

    Boolean * valueOf( bool param0 );

    bool booleanValue(  );

    bool parseBoolean( FAROLPTSTR param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC ApplicationResults: public SMXObject
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    ApplicationResults( FAROLPTSTR param0, long param1 );

    explicit ApplicationResults( LPVOID param0 );

    virtual ~ApplicationResults();

    FAROLPTSTR getResult(  );

    long getTime(  );

public:
};

//***********************************************
class CLASS_DECLSPEC DistanceMode: public SMXObject
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static DistanceMode * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC ADMOnly: public DistanceMode
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    ADMOnly(  );

    explicit ADMOnly( LPVOID param0 );

    virtual ~ADMOnly();

public:
};

//***********************************************
class CLASS_DECLSPEC InterferometerOnly: public DistanceMode
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    InterferometerOnly(  );

    explicit InterferometerOnly( LPVOID param0 );

    virtual ~InterferometerOnly();

public:
};

//***********************************************
class CLASS_DECLSPEC InterferometerSetByADM: public DistanceMode
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    InterferometerSetByADM(  );

    explicit InterferometerSetByADM( LPVOID param0 );

    virtual ~InterferometerSetByADM();

public:
};

//***********************************************
class CLASS_DECLSPEC UserSpecifiedDistance: public DistanceMode
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    UserSpecifiedDistance(  );

    explicit UserSpecifiedDistance( LPVOID param0 );

    virtual ~UserSpecifiedDistance();

public:
};

//***********************************************
class CLASS_DECLSPEC StartTrigger
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static StartTrigger * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC ExternalStartTrigger: public StartTrigger
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    ExternalStartTrigger(  );

    explicit ExternalStartTrigger( LPVOID param0 );

    virtual ~ExternalStartTrigger();

    bool equals( StartTrigger * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC NullStartTrigger: public StartTrigger
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    NullStartTrigger(  );

    explicit NullStartTrigger( LPVOID param0 );

    virtual ~NullStartTrigger();

    bool equals( StartTrigger * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC ContinueTrigger
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static ContinueTrigger * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC NullContinueTrigger: public ContinueTrigger
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    NullContinueTrigger(  );

    explicit NullContinueTrigger( LPVOID param0 );

    virtual ~NullContinueTrigger();

    bool equals( ContinueTrigger * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC DistanceTrigger: public ContinueTrigger
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    DistanceTrigger( double param0 );

    explicit DistanceTrigger( LPVOID param0 );

    virtual ~DistanceTrigger();

    double getDistance(  );

    void setDistance( double param0 );

    bool equals( ContinueTrigger * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC IntervalTrigger: public ContinueTrigger
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    IntervalTrigger( double param0 );

    explicit IntervalTrigger( LPVOID param0 );

    virtual ~IntervalTrigger();

    double getObservationRate(  );

    void setObservationRate( double param0 );

    bool equals( ContinueTrigger * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC ExternalContinueTrigger: public ContinueTrigger
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    ExternalContinueTrigger(  );

    explicit ExternalContinueTrigger( LPVOID param0 );

    virtual ~ExternalContinueTrigger();

    bool equals( ContinueTrigger * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC AsyncMsg: public BaseDriverObject

{
public:
    virtual LPVOID getJavaInstance() = 0;

    static AsyncMsg * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC Diagnostic: public AsyncMsg
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    Diagnostic( int param0, double param1, int param2, FAROLPTSTR param3 );

    explicit Diagnostic( LPVOID param0 );

    virtual ~Diagnostic();

    FAROLPTSTR getTrackerSpecifics(  );

    int getID(  );

    double getTimeStamp(  );

    int getCode(  );

public:
    static const int UNKNOWN; /* 0 */
    static const int POSITION_FAILURE; /* 1 */
    static const int DEVICE_RESET; /* 2 */
    static const int DEVICE_FAILED; /* 3 */
    static const int REAL_TIME_CLOCK_RESET; /* 4 */
    static const int REAL_TIME_CLOCK_NOT_SET; /* 5 */
    static const int DEFAULT_CONFIGURATION_LOADED; /* 6 */
    static const int NO_ENCODER_MAPS; /* 7 */
    static const int INTERNAL_FAILURE; /* 8 */
};

//***********************************************
class CLASS_DECLSPEC Status: public AsyncMsg
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    Status( int param0, bool param1, double param2, int param3, FAROLPTSTR param4 );

    explicit Status( LPVOID param0 );

    virtual ~Status();

    FAROLPTSTR getTrackerSpecifics(  );

    bool getState(  );

    int getID(  );

    double getTimeStamp(  );

    int getCode(  );

public:
    static const int UNKNOWN; /* 0 */
    static const int MEASURING; /* 1 */
    static const int NEED_DISTANCE_RESET; /* 2 */
    static const int LASER_NOT_LOCKED; /* 3 */
    static const int ISOLATOR_OUT_OF_RANGE; /* 4 */
    static const int BAD_WEATHER_DATA; /* 5 */
    static const int MOTORS_OFF; /* 6 */
    static const int MARGINAL_TARGET; /* 7 */
    static const int IFM_DROPOUT; /* 8 */
    static const int ADM_DROPOUT; /* 9 */
    static const int HIGH_RADIAL_SPEED; /* 10 */
    static const int NO_TARGET; /* 11 */
    static const int SEARCHING_FOR_TARGET; /* 12 */
    static const int SHUTTER_CLOSED; /* 13 */
    static const int LASER_OFF; /* 14 */
    static const int LEVEL_ENABLED; /* 15 */
    static const int MOTOR_POSITION_INACCURATE; /* 16 */
    static const int MANUAL_POSITION_CHANGE; /* 17 */
    static const int CLOCK_SET_PENDING; /* 18 */
    static const int CLOSE_TO_LOW_PHYSICAL_STOP; /* 19 */
    static const int CLOSE_TO_HIGH_PHYSICAL_STOP; /* 20 */
    static const int ADM_INACCURATE; /* 21 */
    static const int LICENSE_EXPIRED; /* 22 */
    static const int SMART_WARM_UP_IN_PROGRESS; /* 23 */
    static const int NEED_AUTO_ADJUST; /* 24 */
};

//***********************************************
class CLASS_DECLSPEC Alarm: public AsyncMsg
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    Alarm( int param0, bool param1, double param2, int param3, FAROLPTSTR param4 );

    explicit Alarm( LPVOID param0 );

    virtual ~Alarm();

    FAROLPTSTR getTrackerSpecifics(  );

    int getAlarmNumber(  );

    bool getState(  );

    int getID(  );

    double getTimeStamp(  );

public:
};

//***********************************************
class CLASS_DECLSPEC Change: public AsyncMsg
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    Change( int param0, double param1, int param2, FAROLPTSTR param3, SMXObject * param4 );

    explicit Change( LPVOID param0 );

    virtual ~Change();

    FAROLPTSTR getTrackerSpecifics(  );

    SMXObject * getRelatedData(  );

    int getID(  );

    double getTimeStamp(  );

    int getCode(  );

public:
    static const int UNKNOWN; /* 0 */
    static const int SIGHT_ORIENTATION; /* 1 */
    static const int TRACKING_MODE; /* 2 */
    static const int DISTANCE_MODE; /* 3 */
    static const int TARGET_TYPE; /* 4 */
};

//***********************************************
class CLASS_DECLSPEC Filter
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static Filter * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC NullFilter: public Filter
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    NullFilter(  );

    explicit NullFilter( LPVOID param0 );

    virtual ~NullFilter();

    bool equals( Filter * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC AverageFilter: public Filter
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    AverageFilter(  );

    explicit AverageFilter( LPVOID param0 );

    virtual ~AverageFilter();

    bool equals( Filter * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC StdDevFilter: public Filter
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    StdDevFilter(  );

    explicit StdDevFilter( LPVOID param0 );

    virtual ~StdDevFilter();

    bool equals( Filter * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC AdaptiveFilter: public Filter
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    AdaptiveFilter( double param0 );

    explicit AdaptiveFilter( LPVOID param0 );

    virtual ~AdaptiveFilter();

    double getSdm(  );

    void setSdm( double param0 );

    bool equals( Filter * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC MeasurePointStats
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static MeasurePointStats * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC MeasurePointStdDev: public MeasurePointStats
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit MeasurePointStdDev( LPVOID param0 );

    virtual ~MeasurePointStdDev();

    double azimuthStdDev(  );

    double zenithStdDev(  );

    double distanceStdDev(  );

public:
};

//***********************************************
class CLASS_DECLSPEC MeasurePointData: public BaseDriverObject

{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit MeasurePointData( LPVOID param0 );

    virtual ~MeasurePointData();

    double azimuth(  );

    double zenith(  );

    MeasurePointStats * statistics(  );

    double distance(  );

    double time(  );

    int status(  );

public:
    static const int DATA_ACCURATE; /* 0 */
    static const int DATA_INACCURATE; /* 1 */
    static const int DATA_ERROR; /* 2 */
};

//***********************************************
class CLASS_DECLSPEC MPEResultsData: public BaseDriverObject

{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit MPEResultsData( LPVOID param0 );

    virtual ~MPEResultsData();

    double mpe(  );

    double errorAsPercentageMPE(  );

    bool meetsTrackerSpecification(  );

    double error(  );

public:
};

//***********************************************
class CLASS_DECLSPEC SimplePointPairData
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    SimplePointPairData( double param0, double param1, double param2, double param3, double param4, double param5 );

    explicit SimplePointPairData( LPVOID param0 );

    virtual ~SimplePointPairData();

    double getFirstPointAzimuth(  );

    double getFirstPointZenith(  );

    double getFirstPointDistance(  );

    double getSecondPointAzimuth(  );

    double getSecondPointZenith(  );

    double getSecondPointDistance(  );

public:
};

//***********************************************
class CLASS_DECLSPEC MaterialTemperatureData
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit MaterialTemperatureData( LPVOID param0 );

    virtual ~MaterialTemperatureData();

    double temperature(  );

    int numSensors(  );

public:
};

//***********************************************
class CLASS_DECLSPEC LevelData
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit LevelData( LPVOID param0 );

    virtual ~LevelData();

    double getRX(  );

    double getRY(  );

    double getRZ(  );

public:
};

//***********************************************
class CLASS_DECLSPEC MeasureCfg
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    MeasureCfg( int param0, Filter * param1, StartTrigger * param2, ContinueTrigger * param3 );

    explicit MeasureCfg( LPVOID param0 );

    virtual ~MeasureCfg();

    int getSamplesPerObservation(  );

    void setSamplesPerObservation( int param0 );

    StartTrigger * getStartTrigger(  );

    void setStartTrigger( StartTrigger * param0 );

    ContinueTrigger * getContinueTrigger(  );

    void setContinueTrigger( ContinueTrigger * param0 );

    Filter * getFilter(  );

    void setFilter( Filter * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC TrackerInterface
{
public:
    virtual LPVOID getJavaInstance() = 0;

public:
};

//***********************************************
class CLASS_DECLSPEC TargetType: public SMXObject
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static TargetType * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC SMRTargetType: public TargetType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    SMRTargetType( double param0 );

    explicit SMRTargetType( LPVOID param0 );

    virtual ~SMRTargetType();

    double getDiameter(  );

public:
};

//***********************************************
class CLASS_DECLSPEC MirrorTargetType: public TargetType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    MirrorTargetType(  );

    explicit MirrorTargetType( LPVOID param0 );

    virtual ~MirrorTargetType();

public:
};

//***********************************************
class CLASS_DECLSPEC WindowedSMRTargetType: public TargetType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    WindowedSMRTargetType( double param0 );

    explicit WindowedSMRTargetType( LPVOID param0 );

    virtual ~WindowedSMRTargetType();

    double getDiameter(  );

    double getRadiusOffset( DistanceMode * param0 );

public:
};

//***********************************************
class CLASS_DECLSPEC AlarmType
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static AlarmType * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC DeviationAlarm: public AlarmType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    DeviationAlarm( double param0, double param1 );

    explicit DeviationAlarm( LPVOID param0 );

    virtual ~DeviationAlarm();

    double getSetPoint(  );

    double getDeviation(  );

public:
};

//***********************************************
class CLASS_DECLSPEC HighAlarm: public AlarmType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    HighAlarm( double param0 );

    explicit HighAlarm( LPVOID param0 );

    virtual ~HighAlarm();

    double getSetPoint(  );

public:
};

//***********************************************
class CLASS_DECLSPEC LowAlarm: public AlarmType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    LowAlarm( double param0 );

    explicit LowAlarm( LPVOID param0 );

    virtual ~LowAlarm();

    double getSetPoint(  );

public:
};

//***********************************************
class CLASS_DECLSPEC NullAlarm: public AlarmType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    NullAlarm(  );

    explicit NullAlarm( LPVOID param0 );

    virtual ~NullAlarm();

public:
};

//***********************************************
class CLASS_DECLSPEC ExtTempType
{
public:
    virtual LPVOID getJavaInstance() = 0;

    static ExtTempType * createInstance( LPVOID envVoid, LPVOID objVoid );

public:
};

//***********************************************
class CLASS_DECLSPEC NullExtTemp: public ExtTempType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    NullExtTemp(  );

    explicit NullExtTemp( LPVOID param0 );

    virtual ~NullExtTemp();

public:
};

//***********************************************
class CLASS_DECLSPEC AirExtTemp: public ExtTempType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    AirExtTemp(  );

    explicit AirExtTemp( LPVOID param0 );

    virtual ~AirExtTemp();

public:
};

//***********************************************
class CLASS_DECLSPEC MaterialExtTemp: public ExtTempType
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    MaterialExtTemp(  );

    explicit MaterialExtTemp( LPVOID param0 );

    virtual ~MaterialExtTemp();

public:
};

//***********************************************
class CLASS_DECLSPEC VersionInfo: public SMXObject
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit VersionInfo( LPVOID param0 );

    virtual ~VersionInfo();

    int majorNumber(  );

    int minorNumber(  );

    int minorMinorNumber(  );

    int buildNumber(  );

    FAROLPTSTR specialStr(  );

public:
};

//***********************************************
class CLASS_DECLSPEC WeatherInformation: public SMXObject
{
private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    WeatherInformation( double param0, double param1, double param2 );

    explicit WeatherInformation( LPVOID param0 );

    virtual ~WeatherInformation();

    double getAirTemperature(  );

    double getAirPressure(  );

    double getHumidity(  );

public:
};

//***********************************************
class CLASS_DECLSPEC CommandCompleteEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit CommandCompleteEvent( LPVOID param0 );

    virtual ~CommandCompleteEvent();

    TrackerException * getRelatedException(  );

public:
};

//***********************************************
class CLASS_DECLSPEC ConnectEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit ConnectEvent( LPVOID param0 );

    virtual ~ConnectEvent();

    bool isConnected(  );

public:
};

//***********************************************
class CLASS_DECLSPEC MeasureDataEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit MeasureDataEvent( LPVOID param0 );

    virtual ~MeasureDataEvent();

    int getNumMeasurements(  );

public:
};

//***********************************************
class CLASS_DECLSPEC BkndMeasureDataEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit BkndMeasureDataEvent( LPVOID param0 );

    virtual ~BkndMeasureDataEvent();

    int getNumMeasurements(  );

public:
};

//***********************************************
class CLASS_DECLSPEC DiagnosticEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit DiagnosticEvent( LPVOID param0 );

    virtual ~DiagnosticEvent();

    Diagnostic * getDiagnostic(  );

public:
};

//***********************************************
class CLASS_DECLSPEC StatusEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit StatusEvent( LPVOID param0 );

    virtual ~StatusEvent();

    Status * getStatus(  );

public:
};

//***********************************************
class CLASS_DECLSPEC AlarmEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit AlarmEvent( LPVOID param0 );

    virtual ~AlarmEvent();

    Alarm * getAlarm(  );

public:
};

//***********************************************
class CLASS_DECLSPEC ChangeEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit ChangeEvent( LPVOID param0 );

    virtual ~ChangeEvent();

    Change * getChange(  );

public:
};

//***********************************************
class CLASS_DECLSPEC BusyEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit BusyEvent( LPVOID param0 );

    virtual ~BusyEvent();

    bool getCommandState(  );

    bool getMeasureState(  );

    bool getBkndMeasureState(  );

public:
};

//***********************************************
class CLASS_DECLSPEC ApplicationEvent
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    explicit ApplicationEvent( LPVOID param0 );

    virtual ~ApplicationEvent();

    FAROLPTSTR getAppName(  );

    bool isAppRunning(  );

public:
};
class CLASS_DECLSPEC CommandCompleteListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    CommandCompleteListener(  );

    virtual ~CommandCompleteListener();

    virtual void commandComplete( CommandCompleteEvent * param0 ) = 0;
};
class CLASS_DECLSPEC ConnectListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    ConnectListener(  );

    virtual ~ConnectListener();

    virtual void connectionChanged( ConnectEvent * param0 ) = 0;
};
class CLASS_DECLSPEC MeasureDataListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    MeasureDataListener(  );

    virtual ~MeasureDataListener();

    virtual void dataAvailable( MeasureDataEvent * param0 ) = 0;
};
class CLASS_DECLSPEC BkndMeasureDataListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    BkndMeasureDataListener(  );

    virtual ~BkndMeasureDataListener();

    virtual void dataAvailable( BkndMeasureDataEvent * param0 ) = 0;
};
class CLASS_DECLSPEC DiagnosticListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    DiagnosticListener(  );

    virtual ~DiagnosticListener();

    virtual void diagnosticReported( DiagnosticEvent * param0 ) = 0;
};
class CLASS_DECLSPEC ChangeListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    ChangeListener(  );

    virtual ~ChangeListener();

    virtual void changeOccurred( ChangeEvent * param0 ) = 0;
};
class CLASS_DECLSPEC StatusListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    StatusListener(  );

    virtual ~StatusListener();

    virtual void statusChanged( StatusEvent * param0 ) = 0;
};
class CLASS_DECLSPEC AlarmListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    AlarmListener(  );

    virtual ~AlarmListener();

    virtual void alarmChanged( AlarmEvent * param0 ) = 0;
};
class CLASS_DECLSPEC BusyListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    BusyListener(  );

    virtual ~BusyListener();

    virtual void stateChanged( BusyEvent * param0 ) = 0;
};
class CLASS_DECLSPEC ApplicationListener
{
protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );

private:
    void addStaticPtr( LPVOID envPtr );
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, int peerPtr, LPVOID javaEvent);
public:

    ApplicationListener(  );

    virtual ~ApplicationListener();

    virtual void stateChanged( ApplicationEvent * param0 ) = 0;
};

//***********************************************
class CLASS_DECLSPEC Tracker
{
private:
    static void JNICALL nativeCall( LPVOID env, LPVOID obj, LPVOID appName, bool flag);

protected:
    LPVOID globalInstance;

public:
    LPVOID getJavaInstance();

private:
    LPVOID globalClass;

    void setupGlobalClass( LPVOID envPtr );
public:

    Tracker( TrackerInterface * param0 );

    Tracker( FAROLPTSTR param0 );

    explicit Tracker( LPVOID param0 );

    virtual ~Tracker();

    void initializeSmart(  );

    void initializeSmart( bool param0 );

    void toggleSightOrientation(  );

    void changeMotorState( bool param0 );

    void changeTrackingState( bool param0 );

    void changeDistanceMeasureMode( DistanceMode * param0 );

    void changeTargetType( TargetType * param0 );

    int sampleRate(  );

    void startMeasurePoint( MeasureCfg * param0 );

    TrkDrvObjectArray * readMeasurePointData( int param0 );

    MeasurePointData * readMeasurePointData(  );

    void stopMeasurePoint(  );

    void startBkndMeasurePoint( MeasureCfg * param0 );

    MeasurePointData * readBkndMeasurePointData(  );

    TrkDrvObjectArray * readBkndMeasurePointData( int param0 );

    void stopBkndMeasurePoint(  );

    void startAsync(  );

    void stopAsync(  );

    TrkDrvObjectArray * diagnosticHistory(  );

    TrkDrvObjectArray * statusHistory(  );

    TrkDrvObjectArray * alarmHistory(  );

    bool targetPresent(  );

    bool targetLocationValid(  );

    bool motorsOn(  );

    bool trackingOn(  );

    bool admCapable(  );

    bool admOnlyCapable(  );

    bool interferometerCapable(  );

    bool levelCapable(  );

    DistanceMode * distanceMeasureMode(  );

    TargetType * targetType(  );

    bool backsightOrientation(  );

    LevelData * measureLevel(  );

    MaterialTemperatureData * measureMaterialTemperature(  );

    TrkDrvObjectArray * availableApplications(  );

    void modifyApplication( FAROLPTSTR param0, FAROLPTSTR param1 );

    TrkDrvObjectArray * startedApplications(  );

    void writeFile( FAROLPTSTR param0, FAROLPTSTR param1 );

    void changeAlarmCfg( int param0, AlarmType * param1 );

    AlarmType * alarmCfg( int param0 );

    int numExternalTempSensors(  );

    ExtTempType * externalTempType( int param0 );

    void changeExternalTempType( int param0, ExtTempType * param1 );

    bool externalTempTypeChangeable(  );

    double measureExternalTempSensor( int param0 );

    bool standardPrivilege(  );

    bool adminPrivilege(  );

    bool factoryPrivilege(  );

    void readFileAbort(  );

    void writeFileAbort(  );

    bool admScanCapable(  );

    TrkDrvObjectArray * availableTargetType(  );

    WeatherInformation * getWeatherInfo(  );

    FAROLPTSTR getSerialNumber(  );

    bool readyToInitialize( bool param0 );

    int applicationExitCode( FAROLPTSTR param0 );

    void releaseLock(  );

    void startSlew( double param0, double param1, double param2 );

    void startSlew( double param0, double param1 );

    void stopSlew(  );

    bool isResultReportingCapable(  );

    bool isAdaptiveFilterCapable(  );

    ApplicationResults * applicationResults( FAROLPTSTR param0 );

    bool is_1_5_WindowedSMRTargetCapable(  );

    bool is_7_8_WindowedSMRTargetCapable(  );

    MPEResultsData * checkADMAgainstIFM( SimplePointPairData * param0 );

    MPEResultsData * checkAngularAccuracyError( SimplePointPairData * param0 );

    bool isSelfCompensationNoUICapable(  );

    void setBlocking( bool param0 );

    bool getBlocking(  );

    void setBkndMeasureBlocking( bool param0 );

    bool getBkndMeasureBlocking(  );

    void setMeasureEventRate( int param0 );

    int getMeasureEventRate(  );

    void setBkndMeasureEventRate( int param0 );

    int getBkndMeasureEventRate(  );

    void setExclusiveAccess(  );

    void clearExclusiveAccess(  );

    bool exclusiveAccessSet(  );

    bool exclusiveAccessForCurrentThread(  );

    bool busyForCurrentThread(  );

    bool measureInProgress(  );

    bool bkndMeasureInProgress(  );

    bool asyncStarted(  );

    TrkDrvObjectArray * readBufferedDiagnosticHistory(  );

    TrkDrvObjectArray * readBufferedStatusHistory(  );

    TrkDrvObjectArray * readBufferedAlarmHistory(  );

    LevelData * readBufferedLevel(  );

    MaterialTemperatureData * readBufferedMaterialTemperature(  );

    void startApplicationFrame( FAROLPTSTR param0, FAROLPTSTR param1 );

    void startApplicationFrameAlwaysOnTop( FAROLPTSTR param0, FAROLPTSTR param1 );

    void stopApplicationFrame( FAROLPTSTR param0, bool param1 );

    void stopApplicationFrame( FAROLPTSTR param0 );

    void addCommandCompleteListener( CommandCompleteListener * param0 );

    void removeCommandCompleteListener( CommandCompleteListener * param0 );

    void addConnectListener( ConnectListener * param0 );

    void removeConnectListener( ConnectListener * param0 );

    void addMeasureDataListener( MeasureDataListener * param0 );

    void removeMeasureDataListener( MeasureDataListener * param0 );

    void addBkndMeasureDataListener( BkndMeasureDataListener * param0 );

    void removeBkndMeasureDataListener( BkndMeasureDataListener * param0 );

    void addDiagnosticListener( DiagnosticListener * param0 );

    void removeDiagnosticListener( DiagnosticListener * param0 );

    void addStatusListener( StatusListener * param0 );

    void removeStatusListener( StatusListener * param0 );

    void addAlarmListener( AlarmListener * param0 );

    void removeAlarmListener( AlarmListener * param0 );

    void addBusyListener( BusyListener * param0 );

    void removeBusyListener( BusyListener * param0 );

    void addApplicationListener( ApplicationListener * param0 );

    void removeApplicationListener( ApplicationListener * param0 );

    double readBufferedExternalTempSensor(  );

    int availableMeasurements(  );

    void availableBkndMeasurements(  );

    void runNoUISelfCompensation(  );

    bool abortNoUISelfCompensation(  );

    bool initialized(  );

    bool initialized( bool param0 );

    void search( double param0 );

    void search( double param0, double param1 );

    void initialize( bool param0 );

    void initialize(  );

    void command( FAROLPTSTR param0, int param1 );

    void connect( FAROLPTSTR param0, FAROLPTSTR param1, FAROLPTSTR param2 );

    bool connected(  );

    void move( double param0, double param1, double param2, bool param3 );

    void move( double param0, double param1, double param2, bool param3, bool param4 );

    void readFile( FAROLPTSTR param0, FAROLPTSTR param1 );

    void disconnect(  );

    void addChangeListener( ChangeListener * param0 );

    void removeChangeListener( ChangeListener * param0 );

    VersionInfo * version(  );

    void abort(  );

    void home( bool param0 );

    bool busy(  );

public:
};

#ifdef __cplusplus
}
#endif

#endif /* __TRACKER_H__ */
