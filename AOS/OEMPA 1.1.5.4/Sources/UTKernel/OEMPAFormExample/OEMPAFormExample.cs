//#define _EVALUATION_

using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Globalization;

using System.IO;
using System.Reflection;

using csDriverOEMPA;
using WizardTemplate;

namespace OEMPAFormExample
{
    public partial class OEMPAFormExample : Form
    {
        delegate void delegateUpdateRoot(ref csRoot root);
        private event delegateUpdateRoot mEvent;

        static FormMsgBox msgBox = null;
        static OEMPAFormExample firstDialog=null;
        static int iDialogCount = 0;
        int iThisDialogCount;
        OEMPAFormExample nextDevice;
        OEMPAFormExample prevDevice;

        public csHWDeviceOEMPA hwDeviceOEMPA;
        public int m_iHWDeviceId=-1;
        public bool connect,pulser,bIO;
        public bool bConnectEnter;
        public Int64 dataSizeAscan;
        public Int64 dataSizeCscan;
        public Int64 dataTimerAscan;
        public Int64 dataTimerCscan;
        public int dataTimeAscan, dataTimeCscan;
        public int sizeTime, dataLostAscan, dataLostCscan;
        //static public OEMPAFormExample myForm;
        private /*static*/ object LockingVar = new object();
        public int m_iComboSynchronisation;
        System.Windows.Forms.Timer theTimer;

        private csWizardTemplate wizardTemplate;
        private bool wizardCompleted;
        public string wizardFileName;

        public bool callbackCustomized;
        public bool m_bCallback;
        public String pFileName;
        public int cycleCount;
        double gain, start, range;
        double encoderAscanX, encoderAscanY, encoderCscanX, encoderCscanY;
        public bool bProcessConnection;

        int m_iCycleCount;
        bool[] m_bAcquisitionCscanAmp;
        bool[] m_bAcquisitionCscanTof;
        short[] m_sAcquisitionCscanAmp;
        short[] m_sAcquisitionCscanTof;

        bool checkBoxPulserEnable;

        [Conditional("_EVALUATION_")]
        public void SetThisComputerAddress()
        {
            textBoxIPAddress.Text = "127.0.0.1";
        }

        static bool is64BitProcess = (IntPtr.Size == 8);
        static bool is64BitOperatingSystem = is64BitProcess || InternalCheckIsWow64();

        [DllImport("kernel32.dll", SetLastError = true, CallingConvention = CallingConvention.Winapi)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool IsWow64Process(
            [In] IntPtr hProcess,
            [Out] out bool wow64Process
        );
        [DllImport("kernel32.dll")]
        static extern uint GetCurrentProcessId();

        public static bool InternalCheckIsWow64()
        {
            if ((Environment.OSVersion.Version.Major == 5 && Environment.OSVersion.Version.Minor >= 1) ||
                Environment.OSVersion.Version.Major >= 6)
            {
                using (Process p = Process.GetCurrentProcess())
                {
                    bool retVal;
                    if (!IsWow64Process(p.Handle, out retVal))
                    {
                        return false;
                    }
                    return retVal;
                }
            }
            else
            {
                return false;
            }
        }

        public OEMPAFormExample(OEMPAFormExample previousDevice)
        {
            int iMonitorPort;
            uint ui = GetCurrentProcessId();
            InitializeComponent();

            mEvent += new delegateUpdateRoot (UpdateRoot);
            iThisDialogCount = iDialogCount;
            prevDevice = previousDevice;
            nextDevice = null;
            if(iDialogCount == 0)
            {
                firstDialog = this;
                if (!csHWDeviceOEMPA.IsMultiProcessRegistered())
                    csHWDeviceOEMPA.RegisterMultiProcess("OEMPAFormExample");
            }
            iDialogCount++;

            wizardCompleted = false;
            if (!csWizardTemplate.GetWizardFolder(ref wizardFileName))
                wizardFileName = @"C:\Users\Public\csWizard.txt";
            else
                wizardFileName = wizardFileName + "csWizard.txt";

            bIO = false;
            bConnectEnter = false;
            connect = false;
            pulser = false;
            checkBoxPulserEnable = true;
            dataSizeAscan = 0;
            dataSizeCscan = 0;
            dataTimerAscan = 0;
            dataTimerCscan = 0;
            dataTimeAscan = 0;
            dataTimeCscan = 0;
            callbackCustomized = false;
            m_bCallback = false;
            pFileName = null;
            cycleCount = 0;
            gain = 0.0;
            start = 0.0;
            range = 0.0;
            dataLostAscan = 0;
            dataLostCscan = 0;
            encoderAscanX = 0;
            encoderAscanY = 0;
            encoderCscanX = 0;
            encoderCscanY = 0;
            hwDeviceOEMPA = new csHWDeviceOEMPA();
            m_iHWDeviceId = hwDeviceOEMPA.GetDeviceId();
            m_iCycleCount = 0;

            theTimer = new System.Windows.Forms.Timer();
            //Adds the event and the event handler for the method that will 
            //process the timer event to the timer
            theTimer.Tick += new EventHandler((sender, e) => TimerEventProcess(sender, e, this));
            // Sets the timer interval to 5 seconds
            theTimer.Interval = 1000;
            theTimer.Start();

            UpdateDialog();
            InitDialog();

            textBoxWriteGain.Text = "0.0 dB";
            textBoxWriteStart.Text = "0.0 us";
            textBoxWriteRange.Text = "30.0 us";
            textBoxResolution.Text = "1 step/mm";
            textBoxStep.Text = "1.0 mm";
            comboBoxSynchronisation.SelectedIndex = 0;
            textBoxPort.Text = "5001";
            SetThisComputerAddress();
            bProcessConnection = false;

            if (InternalCheckIsWow64())
            {
                this.Text = "OEMPAFormExample (x86, PID ";
                this.Text += ui.ToString();
                this.Text += ")";
            }
            else
            {
                this.Text = "OEMPAFormExample (PID ";
                this.Text += ui.ToString();
                this.Text += " / ";
                iMonitorPort = csHWDevice.GetMonitorPort(m_iHWDeviceId);
                this.Text += "monitor=";
                this.Text += iMonitorPort.ToString();
                if (iDialogCount > 1)
                {
                    this.Text += " / ";
                    iMonitorPort = csHWDevice.GetMonitorPort(m_iHWDeviceId);
                    this.Text += iDialogCount.ToString();
                }
                this.Text += ")";
            }
            //MessageBox.Show("DevId = " + m_iHWDeviceId.ToString());
        }

        protected override void OnFormClosed(FormClosedEventArgs e)
        {
            if ((nextDevice != null) && (prevDevice != null))
            {
                nextDevice.prevDevice = prevDevice;
                prevDevice.nextDevice = nextDevice;
            }
            if ((nextDevice == null) && (prevDevice != null))
                prevDevice.nextDevice = null;
            iDialogCount--;
            theTimer.Stop();
            hwDeviceOEMPA.Free();
            if (iDialogCount == 0)
                csHWDeviceOEMPA.UnregisterMultiProcess();
        }

        private static void TimerEventProcess(Object myObject, EventArgs myEventArgs, OEMPAFormExample myForm)
        {
            int iComError;
            csSWDevice swDevice;
            String strAux;
            bool connect2, pulser2, dataLost=false;
            Int64 size;
            double dThrougput;
            long dwAscanLostCount, dwCscanLostCount, dwEncoderLostCount, dwUSB3LostCount;

            swDevice = myForm.hwDeviceOEMPA.GetSWDevice();
            connect2 = swDevice.IsConnected();
            pulser2 = swDevice.IsPulserEnabled();
            if (connect2)
                strAux = "Connected";
            else
                strAux = "Disconnected";
            myForm.textBoxStatus.Text = strAux;
            if(connect2)
            lock (myForm.LockingVar)
            {
                if (myForm.bIO && (myForm.m_iComboSynchronisation > 0))
                {
                    strAux = String.Format(" - Encoder: X={0:F1} mm, Y={1:F1} mm", myForm.encoderAscanX * 1000.0, myForm.encoderAscanY * 1000.0);
                    myForm.textBoxStatus.Text += strAux;
                }
                if (myForm.checkBoxPulser.Checked)
                {
                    size = myForm.dataSizeAscan - myForm.dataTimerAscan;
                    if (myForm.dataTimeAscan < myForm.sizeTime)
                    {
                        dThrougput = size * 1000;
                        dThrougput /= (myForm.sizeTime - myForm.dataTimeAscan);
                        dThrougput /= (1024 * 1024);
                    }else
                        dThrougput = 0.0;
                    strAux = String.Format(" - Ascan: {0:F3} MB/s", dThrougput);
                    myForm.textBoxStatus.Text += strAux;
                }
                if (myForm.checkBoxPulser.Checked)
                {
                    size = myForm.dataSizeCscan - myForm.dataTimerCscan;
                    if (myForm.dataTimeCscan < myForm.sizeTime)
                    {
                        dThrougput = size * 1000;
                        dThrougput /= (myForm.sizeTime - myForm.dataTimeCscan);
                        dThrougput /= (1024);
                    }
                    else
                        dThrougput = 0.0;
                    strAux = String.Format(" - Cscan: {0:F3} KB/s", dThrougput);
                    myForm.textBoxStatus.Text += strAux;
                }
                if (myForm.callbackCustomized)
                {
                    myForm.callbackCustomized = false;
                    myForm.textBoxFileName.Text = myForm.pFileName;
                    //myForm.textBoxFileStatus.Text = "Cycles:" + myForm.cycleCount + "  Gain:" + myForm.gain;
                    if (myForm.gain >= 0)
                        strAux = String.Format("Cycles: {0} - Gain: {1:F1} dB - Start: {2:F1} us - Range: {3:F1} us", myForm.cycleCount, myForm.gain, myForm.start * 1e6, myForm.range * 1e6);
                    else
                        strAux = String.Format("Cycles: {0}", myForm.cycleCount);
                    myForm.textBoxFileStatus.Text += strAux;
                }
                iComError = swDevice.GetStreamError();
                if (iComError > 0)
                    myForm.textBoxStatus.Text += " - ComError: " + iComError;
                dwAscanLostCount = swDevice.GetLostCountAscan();
                dwCscanLostCount = swDevice.GetLostCountCscan();
                dwEncoderLostCount = swDevice.GetLostCountEncoder();
                dwUSB3LostCount = swDevice.GetLostCountUSB3();
                if ((dwAscanLostCount > 0) || (dwCscanLostCount > 0) || (dwEncoderLostCount > 0))
                {
                    myForm.textBoxStatus.Text += " - DataLost:";
                    if (dwAscanLostCount > 0)
                    {
                        myForm.textBoxStatus.Text += " A=" + dwAscanLostCount;
                        dataLost = true;
                    }
                    if (dwCscanLostCount > 0)
                    {
                        myForm.textBoxStatus.Text += " C=" + dwCscanLostCount;
                        dataLost = true;
                    }
                    if (dwEncoderLostCount > 0)
                    {
                        myForm.textBoxStatus.Text += " E=" + dwEncoderLostCount;
                        dataLost = true;
                    }
                    if (dwUSB3LostCount > 0)
                    {
                        myForm.textBoxStatus.Text += " U=" + dwUSB3LostCount;
                        dataLost = true;
                    }
                }
            }
            if ((connect2 != myForm.connect) || (pulser2 != myForm.pulser))
            {
                myForm.pulser = pulser2;
                myForm.checkBoxPulserEnable = false;
                myForm.checkBoxPulser.Checked = pulser2;
                myForm.checkBoxPulserEnable = true;
                myForm.connect = connect2;
                myForm.UpdateDialog();
            }
            myForm.dataTimerAscan = myForm.dataSizeAscan;
            myForm.dataTimerCscan = myForm.dataSizeCscan;
            myForm.dataTimeAscan = System.Environment.TickCount;
            myForm.dataTimeCscan = System.Environment.TickCount;
            if ((myForm != null) && (myForm.labelDataLost.Visible != dataLost))
            {
                myForm.labelDataLost.Visible = dataLost;
            }
        }

        private void UpdateDialog()
        {
            bIO = connect && hwDeviceOEMPA.GetSWDeviceOEMPA().IsIOBoardEnabled();

            if(checkBoxConnect.Checked && !connect)
                checkBoxConnect.Checked = false;

            textBoxIPAddress.Enabled = !connect;
            textBoxPort.Enabled = !connect;

            buttonRead.Visible = connect;
            buttonWrite.Visible = connect;
            textBoxRead.Visible = connect;
            textBoxWriteGain.Visible = connect;
            textBoxWriteStart.Visible = connect;
            textBoxWriteRange.Visible = connect;
            buttonRead.Visible = connect;
            buttonWrite.Visible = connect;
            checkBoxPulser.Visible = connect;
            buttonLoad.Visible = connect;
            buttonSave.Visible = connect;
            textBoxFileName.Visible = connect;
            textBoxFileStatus.Visible = connect;
            buttonStatus.Visible = connect;
            buttonMsgBox.Visible = true;
            checkBoxWizardLinear.Visible = connect;

            comboBoxSynchronisation.Visible = connect;
            buttonResetEncoder.Visible = connect;
            buttonWriteResolution.Visible = connect;
            buttonWriteStep.Visible = connect;
            textBoxResolution.Visible = connect;
            textBoxStep.Visible = connect;
            labelResolution.Visible = connect;
            labelStep.Visible = connect;

            buttonTemplate.Visible = connect;
            if ((wizardTemplate == null) || !wizardCompleted)
            {
                buttonWizardToFile.Visible = false;
                buttonWizardToHw.Visible = false;
            }
            else
            {
                buttonWizardToFile.Visible = connect;
                buttonWizardToHw.Visible = connect;
            }

            if ((iThisDialogCount != 0) || (nextDevice == null))
                checkBoxMaster.Visible = false;
            else
            {
                checkBoxMaster.Visible = true;
                if(hwDeviceOEMPA.GetHardwareLink()==csEnumHardwareLink.csMaster)
                    checkBoxMaster.Checked = true;
                else
                    checkBoxMaster.Checked = false;
            }
            if (nextDevice == null)
                buttonNewDevice.Visible = true;
            else
                buttonNewDevice.Visible = false;

            textBoxStatus.Enabled = false;
            textBoxRead.Enabled = false;
            textBoxFileName.Enabled = false;
            textBoxFileStatus.Enabled = false;

            comboBoxSynchronisation.Enabled = bIO;
            buttonResetEncoder.Enabled = bIO;
            buttonWriteResolution.Enabled = bIO;
            buttonWriteStep.Enabled = bIO;
            textBoxResolution.Enabled = bIO;
            textBoxStep.Enabled = bIO;
            labelResolution.Enabled = bIO;
            labelStep.Enabled = bIO;

            m_bCallback = true;
            if (connect && hwDeviceOEMPA.GetSWDeviceOEMPA().IsUSB3Connected())
            {
                groupBoxUSB3.Visible = true;
                radioButtonUSB3.Visible = true;
                radioButtonEthernet.Visible = true;
                radioButtonUSB3.Checked = hwDeviceOEMPA.GetSWDeviceOEMPA().IsUSB3Enabled();
                radioButtonEthernet.Checked = !hwDeviceOEMPA.GetSWDeviceOEMPA().IsUSB3Enabled();
            }
            else
            {
                groupBoxUSB3.Visible = false;
                radioButtonUSB3.Visible = false;
                radioButtonEthernet.Visible = false;
                radioButtonUSB3.Checked = false;
                radioButtonEthernet.Checked = true;
            }
            m_bCallback = false;

        }

        unsafe public int AcquisitionAscan_0x00010103(Object pAcquisitionParameter,ref csAcqInfoEx acqInfo,ref csHeaderStream_0x0001 streamHeader,ref csSubStreamAscan_0x0103 ascanHeader,void* pBufferMax,void* pBufferMin,void* pBufferSat)
        {
            //TO AVOID EXTRA TIME PROCESSING in the callback only fixed memory should be used!
            //https://msdn.microsoft.com/en-us/library/f58wzh21.aspx
            //https://msdn.microsoft.com/en-us/library/zycewsya.aspx
            //https://msdn.microsoft.com/en-us/library/a95009h1.aspx?cs-save-lang=1&cs-lang=csharp#code-snippet-1
            //The following cast is required in case of static function to manage many devices:
            //OEMPAFormExample myForm = (OEMPAFormExample)(pAcquisitionParameter);
            IntPtr address = (IntPtr)pBufferMax;
            byte* ptr = (byte*)address.ToPointer();
            byte[] ascan;

            lock (LockingVar)
            {
                dataSizeAscan += ascanHeader.dataSize * ascanHeader.dataCount;
                encoderAscanX = acqInfo.dEncoder[0];
                encoderAscanY = acqInfo.dEncoder[1];
                sizeTime = System.Environment.TickCount;
                if (ascanHeader.dataSize == 1)
                {
                    ascan = new byte[ascanHeader.dataCount];
                    for (int index = 0; index < ascanHeader.dataCount; index++)
                        ascan[index] = ptr[index];
                }
            }
            return 0;
        }

        unsafe public int AcquisitionCscan_0x00010X02(Object pAcquisitionParameter, ref csAcqInfoEx acqInfo, ref csHeaderStream_0x0001 streamHeader, ref csSubStreamCscan_0x0X02 cscanHeader, ref csCscanAmp_0x0102[] bufferAmp, ref csCscanAmpTof_0x0202[] bufferAmpTof)
        {
            int iCycle,iGateId;
            byte byAmp;
            bool sign;
            short sTof;
            bool bUpdateCscan = false;//true if you want to dump cscan data in the status file.

            lock (LockingVar)
            {
                switch (cscanHeader.version)
                {
                    case 1: dataSizeCscan += cscanHeader.count * sizeof(long); break;
                    case 2: dataSizeCscan += 2 * cscanHeader.count * sizeof(long); break;
                    default: break;
                }
                encoderCscanX = acqInfo.dEncoder[0];
                encoderCscanY = acqInfo.dEncoder[1];
                sizeTime = System.Environment.TickCount;
                if (bUpdateCscan)
                {
                    //for information here is how to get cscan
                    iCycle = cscanHeader.cycle;//get current cycle for which cscan are delivered.
                    if (cscanHeader.version == 1)
                    {
                        //cscan amplitude only
                        for (int iDataIndex = 0; iDataIndex < cscanHeader.count; iDataIndex++)
                        {
                            iGateId = bufferAmp[iDataIndex].gateId;//get the gateId: 0=first gate, 1=second gate, ...
                            byAmp = bufferAmp[iDataIndex].byAmp;//get the amplitude
                            sign = bufferAmp[iDataIndex].sign;//get the sign: false=unsigned data, true=signed data.
                            if (m_bAcquisitionCscanAmp != null)
                                m_bAcquisitionCscanAmp[iGateId + 4 * iCycle] = true;
                            if (m_sAcquisitionCscanAmp != null)
                                m_sAcquisitionCscanAmp[iGateId + 4 * iCycle] = byAmp;
                        }
                    }
                    else if (cscanHeader.version == 2)
                    {
                        //cscan amplitude + time of flight
                        for (int iDataIndex = 0; iDataIndex < cscanHeader.count; iDataIndex++)
                        {
                            iGateId = bufferAmpTof[iDataIndex].gateId;//get the gateId: 0=first gate, 1=second gate, ...
                            byAmp = bufferAmpTof[iDataIndex].byAmp;//get the amplitude
                            sign = bufferAmpTof[iDataIndex].sign;//get the sign: false=unsigned data, true=signed data.
                            sTof = (short)bufferAmpTof[iDataIndex].wTof;//get the time of flight
                            if (m_bAcquisitionCscanAmp != null)
                                m_bAcquisitionCscanAmp[iGateId + 4 * iCycle] = true;
                            if (m_sAcquisitionCscanAmp != null)
                                m_sAcquisitionCscanAmp[iGateId + 4 * iCycle] = byAmp;
                            if (m_bAcquisitionCscanTof != null)
                                m_bAcquisitionCscanTof[iGateId + 4 * iCycle] = true;
                            if (m_sAcquisitionCscanTof != null)
                                m_sAcquisitionCscanTof[iGateId + 4 * iCycle] = sTof;
                        }
                    }
                }

            }
            return 0;
        }

        public int AcquisitionIO_0x00010101(Object pAcquisitionParameter,ref csHeaderStream_0x0001 streamHeader,ref csHeaderIO_0x0001 ioHeader)
        {
            return 0;
        }

        public int AcquisitionInfo(Object pAcquisitionParameter, String pInfo)
        {
            if (pInfo.IndexOf("HW Error : acquisition data lost") == 0)
            {
                //to avoid to display "acquisition data lost" with a pop-up (otherwise because of the ballsnow effect you will have a lot of new acq data lost).
                if (pInfo.IndexOf("A") > 0 || pInfo.IndexOf("A+C") > 0 || pInfo.IndexOf("FW FIFO") > 0)
                    dataLostAscan++;
                if (pInfo.IndexOf("C") > 0 || pInfo.IndexOf("A+C") > 0)
                    dataLostCscan++;
                return 0;
            }
            if (pInfo.IndexOf("HW Error : stream bad address") == 0)
                return 0;
            MessageBox.Show(pInfo);
            return 0;
        }

        public int CallbackCustomizedAPI(Object pAcquisitionParameter, ref csEnumStepCustomizedAPI eStep, String pFile, ref int iCycleCount)
        {
            csCustomizedAPI api;
            csCycle cycle;
            csFocalLaw focalLaw;
            bool bWriteHWUpdate = false;//false if you dont want update data before updating the HW device, true if you want to update data.
            bool bFileSaveUpdate = false;//false if you dont want update data before saving OEMPA file, true if you want to update data.
            csRoot root;
            bool bRet = true;

            api = hwDeviceOEMPA.GetCustomizedAPI();
            if (iCycleCount > 0)
            {
                switch (eStep)
                {
                    case csEnumStepCustomizedAPI.csWriteFile_Enter://called just before saving OEMPA file.
                        if (api.GetRoot(out root))
                        {
                            if (bFileSaveUpdate)//it is possible to change parameters before saving the OEMPA file.
                            {
                                root.ullSavedParameters |= (ulong)csEnumCustomizedFlags.csOEMPA_CSCAN_ENABLE_TOF | (ulong)csEnumCustomizedFlags.csOEMPA_ASCAN_REQUEST;
                                root.bEnableCscanTof = true;
                                root.csAscanRequest = csEnumAscanRequest.eAscanSampled;
                                root.dAscanRequestFrequency = 491.0;
                                api.SetRoot(ref root);
                            }
                        }
                        break;
                    case csEnumStepCustomizedAPI.csWriteHW_Enter:
                        callbackCustomized = true;
                        pFileName = pFile;
                        cycleCount = iCycleCount;
                        if (api.GetRoot(out root))
                        {
                            //1.1.5.3l takes too much time: delegateUpdateRoot d = this.UpdateRoot;
                            //1.1.5.3l takes too much time: this.Invoke(d, new object[] { root });
                            mEvent.BeginInvoke(ref root,null,null);
                            if (root.iCycleCount > 0)
                            {
                                m_iCycleCount = root.iCycleCount;
                                m_bAcquisitionCscanAmp = new bool[4*m_iCycleCount];
                                m_bAcquisitionCscanTof = new bool[4*m_iCycleCount];
                                m_sAcquisitionCscanAmp = new short[4*m_iCycleCount];
                                m_sAcquisitionCscanTof = new short[4*m_iCycleCount];
                                for (int iGate2 = 0; iGate2 < 4; iGate2++)
                                {
                                    for (int iCycle2 = 0; iCycle2 < m_iCycleCount; iCycle2++)
                                    {
                                        m_bAcquisitionCscanAmp[iGate2+4*iCycle2] = false;
                                        m_bAcquisitionCscanTof[iGate2 + 4 * iCycle2] = false;
                                        m_sAcquisitionCscanAmp[iGate2 + 4 * iCycle2] = 0;
                                        m_sAcquisitionCscanTof[iGate2 + 4 * iCycle2] = 0;
                                    }
                                }
                            }
                            else
                            {
                                m_iCycleCount = 0;
                                m_bAcquisitionCscanAmp = null;
                                m_bAcquisitionCscanTof = null;
                                m_sAcquisitionCscanAmp = null;
                                m_sAcquisitionCscanTof = null;;
                            }
                        }
                        gain = -1.0;
                        if (api.GetCycle(0, out cycle))
                        {
                            gain = cycle.dGainDigital;//read the amplification (first cycle).
                            start = cycle.dStart;
                            range = cycle.dRange;
                            if (bWriteHWUpdate)//it is possible to change parameters before updating the HW with them.
                            {
                                cycle.dGainDigital = 20.0;//update the amplification.
                                if (!api.SetCycle(0, cycle))
                                    bRet = false;
                            }
                        }
                        if (api.GetFocalLaw(true, 0, out focalLaw))
                        {
                            focalLaw.afPrm[0] += 0.0f;
                            api.SetFocalLaw(true, 0, focalLaw);
                        }
                        break;
                }
            }
            Unreferenced.Parameter(bRet);
            return 0;
        }

        private void UpdateRoot(ref csRoot root)
        {
            m_bCallback = true;
            if (root.csExternalTriggerSequence != csEnumDigitalInput.csDigitalInputOff)
            {
                if ((root.csRequestIO != csEnumOEMPARequestIO.csOEMPANotRequired) &&
                    (root.csRequestIO != csEnumOEMPARequestIO.csOEMPAOnDigitalInputOnly))
                {
                    comboBoxSynchronisation.SelectedIndex = 3;
                    textBoxResolution.Text = String.Format("{0:F2} step/mm", (double)root.lSWEncoderResolution1 / (double)root.dwSWEncoderDivider1);
                }
            }
            else
            {
                switch (root.csTriggerMode)
                {
                    case csEnumOEMPATrigger.csOEMPAInternal:
                        comboBoxSynchronisation.SelectedIndex = 0;
                        break;
                    case csEnumOEMPATrigger.csOEMPAEncoder:
                        if ((root.csRequestIO != csEnumOEMPARequestIO.csOEMPANotRequired) &&
                            (root.csRequestIO != csEnumOEMPARequestIO.csOEMPAOnDigitalInputOnly))
                        {
                            comboBoxSynchronisation.SelectedIndex = 1;
                            textBoxResolution.Text = String.Format("{0:F2} step/mm", (double)root.lSWEncoderResolution1 / (double)root.dwSWEncoderDivider1);
                            textBoxStep.Text = String.Format("{0:F2} mm", root.dTriggerEncoderStep * 1.0e3);
                        }
                        break;
                    default:
                        if ((root.csRequestIO != csEnumOEMPARequestIO.csOEMPANotRequired) &&
                            (root.csRequestIO != csEnumOEMPARequestIO.csOEMPAOnDigitalInputOnly))
                        {
                            comboBoxSynchronisation.SelectedIndex = 2;
                            textBoxResolution.Text = String.Format("{0:F2} step/mm", (double)root.lSWEncoderResolution1 / (double)root.dwSWEncoderDivider1);
                        }
                        break;
                }
            }
            m_iComboSynchronisation = comboBoxSynchronisation.SelectedIndex;
            m_bCallback = false;
        }

        unsafe private void InitDialog()
        {
            csHWDevice hwDevice;
            csAcquisitionFifo fifoAscan, fifoCscan;
            csCustomizedAPI api;
            bool bFifoAscanEnable=false;
            bool bFifoCscanEnable=false;
            Int64 iBufferByteSize=(Int64)2*(Int64)8*(Int64)1024*(Int64)1024;

            if (hwDeviceOEMPA == null)
                return;
            hwDevice = hwDeviceOEMPA.GetHWDevice();
			fifoAscan = hwDevice.GetAcquisitionFifo(csEnumAcquisitionFifo.csFifoAscan);
            if (bFifoAscanEnable && !fifoAscan.Alloc(1024, iBufferByteSize))//function to enable the fifo for ascan.
                MessageBox.Show("Error fifoAscan.Alloc");
            fifoCscan = hwDevice.GetAcquisitionFifo(csEnumAcquisitionFifo.csFifoCscan);
			if(bFifoCscanEnable && !fifoCscan.Alloc(1024, 1024*1024))//function to enable the fifo for cscan.
                MessageBox.Show("Error fifoCscan.Alloc");
            hwDevice.SetAcquisitionParameter(this);
            hwDevice.SetAcquisitionAscan_0x00010103(AcquisitionAscan_0x00010103);
            hwDevice.SetAcquisitionCscan_0x00010X02(AcquisitionCscan_0x00010X02);
            hwDevice.SetAcquisitionIO_0x00010101(AcquisitionIO_0x00010101, false);
            hwDevice.SetAcquisitionInfo(AcquisitionInfo);
            api = hwDeviceOEMPA.GetCustomizedAPI();
            api.SetCallbackCustomizedDriverAPI(CallbackCustomizedAPI);
            //hwDeviceOEMPA.test();//test function is used to test all callback.
        }

        private void checkBoxConnect_CheckedChanged(object sender, EventArgs e)
        {
            csHWDevice hwDevice;
            csSWDevice swDevice;
            csSWDeviceOEMPA swDeviceOEMPA;
            String strIPAddress=textBoxIPAddress.Text;
            ushort usValue;
            bool bRet = true;
            UInt32 dwProcessId;

            try
            {
                usValue = Convert.ToUInt16(textBoxPort.Text);
            }
            catch(Exception ex)
            {
                Unreferenced.Parameter(ex);
                usValue = 5001;
                bRet = false;
            }
            if (!bRet)
            {
                MessageBox.Show("Error to convert string to ushort!");
                return;
            }
            if (hwDeviceOEMPA == null)
                return;
            if(bConnectEnter)
                return;
            bConnectEnter = true;
            //theTimer.Stop();
            hwDevice = hwDeviceOEMPA.GetHWDevice();
            swDevice = hwDeviceOEMPA.GetSWDevice();
            swDeviceOEMPA = hwDeviceOEMPA.GetSWDeviceOEMPA();
            if (checkBoxConnect.Checked)
            {
                if (!swDevice.IsConnected())
                {
                    if (csHWDeviceOEMPA.IsMultiProcessConnected(strIPAddress, out dwProcessId))
                        csHWDeviceOEMPA.DisconnectMultiProcess(strIPAddress, dwProcessId);
                    dataLostAscan = 0;
                    dataLostCscan = 0;
                    bRet = swDeviceOEMPA.SetIPAddress(strIPAddress);
                    bRet = bRet && swDeviceOEMPA._SetPort(usValue);
                    bRet = bRet && swDevice.SetConnectionState(csEnumConnectionState.csConnected,true);
                    if (bRet)
                    {
                        IniFile ini = new IniFile(wizardFileName);
                        bProcessConnection = true;
                        checkBoxWizardLinear.Checked = true;
                        if(ini.IsTemplateSector())
                             checkBoxWizardLinear.Checked = false;
                        bProcessConnection = false;
                    }
                }
            }
            else
            {
                if (swDevice.IsConnected())
                {
                    bRet = swDevice.SetConnectionState(csEnumConnectionState.csDisconnected, true);
                }
            }
            //theTimer.Enabled = true;
            if (swDevice.IsConnected())
                checkBoxConnect.Checked = true;
            else
                checkBoxConnect.Checked = false;
            bConnectEnter = false;
            if (!bRet)
                MessageBox.Show("Communication error!");
        }
        
        private void buttonRead_Click(object sender, EventArgs e)
        {
            double dGain = -1.0;
            double dStart = -1.0;
            double dRange = -1.0;
            int iCycleCount=-1;
            bool bRet = true;

            unsafe
            {
                if (hwDeviceOEMPA.LockDevice())
                {
                    if (!hwDeviceOEMPA.GetCycleCount(&iCycleCount))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetGainDigital(0, &dGain))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetAscanStart(0, &dStart))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetAscanRange(0, &dRange, (csEnumCompressionType*)0, (int*)0, (int*)0))
                        bRet = false;
                    if (!hwDeviceOEMPA.UnlockDevice())
                        bRet = false;
                }
                else
                    bRet = false;
            }
            if (!bRet)
                MessageBox.Show("Communication error!");
            else
            {
                //textBoxRead.Text = "Cycles: " + iCycleCount + " - Gain: " + dGain + " dB" + " - Start: " + dStart + " us" + " - Range: " + dRange + " us";
                textBoxRead.Text = String.Format("Cycles: {0} - Gain: {1:F1} dB - Start: {2:F1} us - Range: {3:F1} us", iCycleCount, dGain, dStart * 1e6, dRange * 1e6);
            }
        }

        private void Test1()
        {
            //test purpose
            csEnumOEMPAFilter eFilter = csEnumOEMPAFilter.csOEMPAHighPassOrder64Cut1Mhz;
            csEnumOEMPAFilterIndex eFilterIndex = csEnumOEMPAFilterIndex.csOEMPAFilter1;
            UInt16 wScale, wScale2;
            Int16[] wValue, wValue2;
            acsDac dacSlope;
            acsDac dacSlope2;
            acsDac dacGain;
            csDac item;
            bool bRet = true;
            int iDeviceId;
            int iCycleCount;
            bool[] abTrackingEnable;
            int[] aiTrackingGateIndex;
            int[] aiTrackingCycleIndex;
            UInt32[] adwHWAperture;
            acsFloat EDelays = new acsFloat();
            acsFloat EWidths = new acsFloat();
            acsFloat RGains = new acsFloat();
            acsDelayReception RDelays = new acsDelayReception();
            acsDouble tof = new acsDouble();
            acsDouble tof2=null;

            adwHWAperture = new UInt32[4];
            adwHWAperture[0] = 0xffffffff;
            adwHWAperture[1] = 0xffffffff;
            adwHWAperture[2] = 0x00000000;
            adwHWAperture[3] = 0x00000000;
            EDelays.list = new float[64];
            for (int i = 0; i < 64; i++)
                EDelays.list[i] = i * 4.0e-9f;
            RDelays.list = new float[64, 1];
            for (int i = 0; i < 64; i++)
                RDelays.list[i, 0] = i * 5.0e-9f;
            EWidths.list = new float[64];
            for (int i = 0; i < 64; i++)
                EWidths.list[i] = 100.0e-9f;
            RGains.list = new float[64];
            for (int i = 0; i < 64; i++)
                RGains.list[i] = 0.0f;
            iDeviceId = hwDeviceOEMPA.GetDeviceId();
            wValue2 = new Int16[5];
            wScale2 = 0;
            dacSlope = new acsDac();
            dacSlope.list = new csDac[4];
            dacSlope.list[0] = new csDac(1e-6, 0.1e6f);//1 us, 0.1 dB/us
            dacSlope.list[1] = new csDac(2e-6, 0.2e6f);//2 us, 0.2 dB/us
            dacSlope.list[2] = new csDac(3e-6, 0.3e6f);//3 us, 0.3 dB/us
            dacSlope.list[3] = new csDac(4e-6, 0.4e6f);//4 us, 0.4 dB/us
            item = dacSlope.op_Subscript(0);
            dacGain = new acsDac();
            dacGain.list = new csDac[4];
            dacGain.list[0] = new csDac(1e-6, 0.0f);//1 us, 0.0 dB (first point should be 0 dB).
            dacGain.list[1] = new csDac(2e-6, 1.0f);//2 us, 1.0 dB
            dacGain.list[2] = new csDac(3e-6, 2.0f);//3 us, 2.0 dB
            dacGain.list[3] = new csDac(4e-6, 3.0f);//4 us, 3.0 dB
            tof.list = new double[1];
            tof.list[0] = 12.3e-6;//12.3 us
            unsafe
            {
		        if(!hwDeviceOEMPA.GetFilterCoefficient(eFilter,&wScale,out wValue))
                    bRet = false;
                else
                if (hwDeviceOEMPA.LockDevice())
                {
                    if (!hwDeviceOEMPA.SetEmissionEnable(0, ref adwHWAperture))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionEnable(0, ref adwHWAperture))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetEmissionWidths(0, ref adwHWAperture, ref EWidths))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetEmissionDelays(0, ref adwHWAperture, ref EDelays))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionGains(0, ref adwHWAperture, ref RGains))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionDelays(0, ref adwHWAperture, ref RDelays))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetFilter(eFilterIndex, ref wScale, ref wValue))
                        bRet = false;
                    //SetDACSlope or SetDACGain as you want:
                    //if (!hwDeviceOEMPA.SetDACSlope(0, ref dacSlope))
                    //    bRet = false;
                    if (!hwDeviceOEMPA.SetDACGain(true, 0, ref dacGain))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionFocusing(0, ref tof))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetCycleCount(&iCycleCount))
                        bRet = false;
                    if (!hwDeviceOEMPA.Flush())
                        bRet = false;
                    if (!hwDeviceOEMPA.GetReceptionFocusing(0, out tof2))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetFilter(eFilterIndex, &wScale2, out wValue2))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetDACSlope(0, out dacSlope2))
                        bRet = false;
                    if (iCycleCount > 0)
                    {
                        abTrackingEnable = new bool[iCycleCount * 4];
                        aiTrackingGateIndex = new int[iCycleCount * 4];
                        aiTrackingCycleIndex = new int[iCycleCount * 4];
                        if ((abTrackingEnable != null) && (abTrackingEnable != null) && (abTrackingEnable != null))
                        {
                            fixed (bool* pbFixedTrackingEnable = &abTrackingEnable[0])
                            {
                                fixed (int* piFixedTrackingGateIndex = &aiTrackingGateIndex[0], piFixedTrackingCycleIndex = &aiTrackingCycleIndex[0])
                                {
                                    for (int iCycle = 0; iCycle < iCycleCount; iCycle++)
                                    {
                                        for (int iGate = 0; iGate < 4; iGate++)
                                        {
                                            if (!hwDeviceOEMPA.GetTrackingGateStart(iCycle, iGate, &pbFixedTrackingEnable[iCycle * 4 + iGate], &piFixedTrackingCycleIndex[iCycle * 4 + iGate], &piFixedTrackingGateIndex[iCycle * 4 + iGate]))
                                                bRet = false;
                                        }
                                    }
                                    if (!hwDeviceOEMPA.UnlockDevice())
                                        bRet = false;
                                }
                            }
                        }
                    }
                }
                else
                    bRet = false;
            }
            if (!bRet)
                MessageBox.Show("Communication error!");
            else
            {
                if (wValue.Length != wValue2.Length)
                    MessageBox.Show("Compare error!");
                else{
                    for (int i = 0; i < wValue.Length; i++)
                    {
                        if (wValue[i] != wValue2[i])
                            MessageBox.Show("Compare error!");
                    }
                }
                if (wScale != wScale2)
                    MessageBox.Show("Compare error!");
                if (tof != tof2)
                    MessageBox.Show("Compare error!");
            }
        }

        private void Test2()
        {//example through transmission probe element 1 => elemnt 2
            bool bRet = true;
            int iDeviceId, iCycleCount;
            double dWedgeDelay = 0.0;
            bool bEnableDDF;
            UInt32[] adwEHWAperture;//Emission aperture (element #1)
            UInt32[] adwRHWAperture;//Reception aperture (element #2)
            acsFloat EDelays = new acsFloat();
            acsFloat EWidths = new acsFloat();
            acsFloat RGains = new acsFloat();
            acsDelayReception RDelays = new acsDelayReception();
            acsDouble tof = new acsDouble();

            MessageBox.Show("Warning: before calling this function you should load an OEMPA file with one cycle to initialize all UT Parameters for those two cycles!");
            adwEHWAperture = new UInt32[4];
            adwEHWAperture[0] = 0x00000001;//element #1
            adwEHWAperture[1] = 0x00000000;
            adwEHWAperture[2] = 0x00000000;
            adwEHWAperture[3] = 0x00000000;
            adwRHWAperture = new UInt32[4];
            adwRHWAperture[0] = 0x00000002;//element #2
            adwRHWAperture[1] = 0x00000000;
            adwRHWAperture[2] = 0x00000000;
            adwRHWAperture[3] = 0x00000000;
            EDelays.list = new float[1];
            EDelays.list[0] = 0.0f;
            RDelays.list = new float[1/*64*/, 1];
            RDelays.list[0, 0] = 0.0f;
            EWidths.list = new float[1];
            EWidths.list[0] = 100.0e-9f;
            RGains.list = new float[64];
            RGains.list[0] = 0.0f;
            tof.list = new double[1];
            tof.list[0] = 0.0e-6;//0.0 us
            bEnableDDF = false;
            iCycleCount = 1;
            iDeviceId = hwDeviceOEMPA.GetDeviceId();
            unsafe
            {
                if (hwDeviceOEMPA.LockDevice())
                {
                    //cycle 1
                    if (!hwDeviceOEMPA.SetEmissionWedgeDelay(0, iCycleCount, ref dWedgeDelay))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionWedgeDelay(0, iCycleCount, ref dWedgeDelay))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetEmissionEnable(0, ref adwEHWAperture))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionEnable(0, ref adwRHWAperture))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetEmissionWidths(0, ref adwEHWAperture, ref EWidths))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetEmissionDelays(0, ref adwEHWAperture, ref EDelays))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionGains(0, ref adwRHWAperture, ref RGains))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionDelays(0, ref adwRHWAperture, ref RDelays))
                        bRet = false;
                    if (!hwDeviceOEMPA.EnableDDF(0, ref bEnableDDF))
                        bRet = false;
                    if (!hwDeviceOEMPA.SetReceptionFocusing(0, ref tof))
                        bRet = false;
                    //CycleCount
                    if (!hwDeviceOEMPA.SetCycleCount(ref iCycleCount))
                        bRet = false;
                    if (!hwDeviceOEMPA.UnlockDevice())
                        bRet = false;
                }else
                    bRet = false;
            }
            if (!bRet)
                MessageBox.Show("Communication error!");
        }

        private void Test3()
        {//example through transmission probe element 1 => elemnt 2
            bool bRet = true;
            int iCycleCount;
            acsFloat EWidths = new acsFloat();

            MessageBox.Show("Warning: before calling this function you should load an OEMPA file with one cycle to initialize all UT Parameters for those two cycles!");
            iCycleCount = 1;
            unsafe
            {
                if (hwDeviceOEMPA.LockDevice())
                {
                    if (!hwDeviceOEMPA.GetCycleCount(&iCycleCount))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetEmissionWidths(0, out EWidths))
                        bRet = false;
                    if (!hwDeviceOEMPA.UnlockDevice())
                        bRet = false;
                }
                else
                    bRet = false;
            }
            if (!bRet)
                MessageBox.Show("Communication error!");
        }

        public csHWDeviceOEMPA _hwDeviceOEMPA;
        public int _cycleCount;
        public bool WriteGateThreshold(int iCycle, int iGate, bool bEnable,
                    csEnumGateModeAmp eModeAmp, csEnumGateModeTof eModeTof,
                    csEnumRectification eRecti, double dThresholdPercent)
        {
            bool bRet = true;

            try
            {
                if (_hwDeviceOEMPA.LockDevice())
                {
                    for (int i = 0; i < _cycleCount; i++)
                    {
                        if (!_hwDeviceOEMPA.SetGateModeThreshold(i, iGate, ref bEnable, ref eModeAmp, ref eModeTof,
                            ref eRecti, ref dThresholdPercent))
                        {
                            bRet = false;
                            break;
                        }
                    }
                    if (!_hwDeviceOEMPA.UnlockDevice())
                        bRet = false;
                }
                else
                    bRet = false;
            }
            catch
            {
                bRet = false;
            }
            return bRet;
        }

        public void CallbackHWMemory(csHWDevice hwDevice, bool bMaster, UInt32 addr, UInt32 data, UInt32 size)
        {
            if (addr == 0x30)
                addr = 0x30;
        }
        private void buttonTest_Click(object sender, EventArgs e)
        {
            //hwDeviceOEMPA.SetCallbackHWMemory(CallbackHWMemory);
            Test9();
        }

        public bool GetGainStartRange(out double dGain, out double dStart, out double dRange)
        {
            bool bRet = false;

            dGain = 0.0;
            dStart = 0.0;
            dRange = 0.0;
            if (ConvertToDouble(textBoxWriteGain.Text, 1.0, out dGain) &&
                ConvertToDouble(textBoxWriteStart.Text, 1.0, out dStart) &&
                ConvertToDouble(textBoxWriteRange.Text, 1.0, out dRange))
            {
                textBoxWriteGain.Text = dGain.ToString(CultureInfo.InvariantCulture) + " dB";
                textBoxWriteStart.Text = dStart.ToString(CultureInfo.InvariantCulture) + " us";
                textBoxWriteRange.Text = dRange.ToString(CultureInfo.InvariantCulture) + " us";
                bRet = true;
                dStart = dStart * 1.0e-6;
                dRange = dRange * 1.0e-6;
            }
            return bRet;
        }
        public bool SetGainStartRange(double dGain, double dStart, double dRange)
        {
            bool bRet = true;
            int iPointCount, iPointFactor;
            csEnumCompressionType eCompressionType;

            eCompressionType = csEnumCompressionType.csCompression;
            if (!hwDeviceOEMPA.SetGainDigital(0, ref dGain))
                bRet = false;
            if (!hwDeviceOEMPA.SetAscanStart(0, ref dStart))
                bRet = false;
            iPointCount = 0;
            iPointFactor = 0;
            if (!hwDeviceOEMPA.SetAscanRange(0, ref dRange, out eCompressionType, out iPointCount, out iPointFactor))
                bRet = false;
            return bRet;
        }

        private void buttonWrite_Click(object sender, EventArgs e)
        {
            double dGain, dStart, dRange;
            bool bRet = true;

            if (checkBoxMaster.Checked || (prevDevice!=null))
            {
                buttonWrite_MasterClick();
                return;
            }
            //use Lock/Unlock for each device
            if (!GetGainStartRange(out dGain, out dStart, out dRange))
            {
                MessageBox.Show("Error to convert string to double.");
                return;
            }
            if (hwDeviceOEMPA.LockDevice())
            {
                bRet = SetGainStartRange(dGain, dStart, dRange);
                if (!hwDeviceOEMPA.UnlockDevice())
                    bRet = false;
            }
            else
                bRet = false;
            if (!bRet)
                MessageBox.Show("Communication error!");
        }

        private void checkBoxPulser_CheckedChanged(object sender, EventArgs e)
        {
            bool bRet = true;
            bool bEnable;
            csSWDevice swDevice;
            //int iDeviceId;

            //iDeviceId = hwDeviceOEMPA.GetDeviceId();
            //if(m_iHWDeviceId!=iDeviceId)
            //    MessageBox.Show("DevId1 = " + m_iHWDeviceId.ToString() + "DevId2 = " + iDeviceId.ToString());
            //else
            //    MessageBox.Show("DevId = " + m_iHWDeviceId.ToString());
            if (!checkBoxPulserEnable)
                return;
            if (checkBoxPulser.Checked)
                bEnable = true;
            else
                bEnable = false;
            swDevice = hwDeviceOEMPA.GetSWDevice();
            bRet = swDevice.EnablePulser(bEnable);
            //another way to enable pulser:
            //if (hwDeviceOEMPA.LockDevice())
            //{
            //    if (!hwDeviceOEMPA.UnlockDevice(csEnumAcquisitionState::csAcqOn))
            //        bRet = false;
            //}
            //else
            //    bRet = false;
            if (!bRet)
                MessageBox.Show("Communication error!");
            else
                pulser = checkBoxPulser.Checked;
        }

        private void buttonLoad_Click(object sender, EventArgs e)
        {
            String strFileName;
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            //openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "txt files (*.txt)|*.txt|binary files (*.bin)|*.bin|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 1;
            //openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                strFileName = openFileDialog1.FileName;
                csCustomizedAPI _csCustomizedAPI = hwDeviceOEMPA.GetCustomizedAPI();
                _csCustomizedAPI.ReadFileWriteHW(strFileName);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            String strFileName;
            SaveFileDialog dlg = new SaveFileDialog();

            //dlg.InitialDirectory = "c:\\";
            dlg.Filter = "txt files (*.txt)|*.txt|binary files (*.bin)|*.bin|All files (*.*)|*.*";
            dlg.FilterIndex = 1;
            //dlg.RestoreDirectory = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                strFileName = dlg.FileName;
                csCustomizedAPI _csCustomizedAPI = hwDeviceOEMPA.GetCustomizedAPI();
                _csCustomizedAPI.ReadHWWriteFile(strFileName);
            }
        }

        private void comboBoxSynchronisation_SelectedIndexChanged(object sender, EventArgs e)
        {
            csEnumOEMPATrigger eTrig = csEnumOEMPATrigger.csOEMPAInternal;
            csEnumOEMPARequestIO eReqIO = csEnumOEMPARequestIO.csOEMPAOnDigitalInputOnly;
            csEnumDigitalInput digInput;
            csEnumEncoderType eEncoder1Type, eEncoder2Type;
	        bool bRet=true;

            if (m_bCallback)
                return;
            if (!hwDeviceOEMPA.GetSWDevice().IsConnected())
                return;
	        switch(comboBoxSynchronisation.SelectedIndex)
	        {
            case 3:
                eTrig = csEnumOEMPATrigger.csOEMPAExternalSequence;
                eReqIO = csEnumOEMPARequestIO.csOEMPAOnDigitalInputAndCycle;
                break;
            case 2:
		        eTrig = csEnumOEMPATrigger.csOEMPAInternal;
		        eReqIO = csEnumOEMPARequestIO.csOEMPAOnDigitalInputAndCycle;
		        break;
	        case 1:
		        eTrig = csEnumOEMPATrigger.csOEMPAEncoder;
		        eReqIO = csEnumOEMPARequestIO.csOEMPAOnDigitalInputAndCycle;
		        break;
	        case 0:
		        eTrig = csEnumOEMPATrigger.csOEMPAInternal;
		        eReqIO = csEnumOEMPARequestIO.csOEMPAOnDigitalInputOnly;
                //bFreeDigitalInput = true;//right call here (and for case 2)
		        break;
	        }
            if ((m_iComboSynchronisation == 0) && (comboBoxSynchronisation.SelectedIndex > 0))
            {
                buttonWriteResolution_Click(sender, e);
                buttonWriteStep_Click(sender, e);
            }
            if (hwDeviceOEMPA.LockDevice())
            {
                //first free digital inputs
                digInput = csEnumDigitalInput.csDigitalInputOff;
                if (!hwDeviceOEMPA.SetEncoderWire1(0, ref digInput))
                    bRet = false;
                if (!hwDeviceOEMPA.SetEncoderWire2(0, ref digInput))
                    bRet = false;
                if (!hwDeviceOEMPA.SetEncoderWire1(1, ref digInput))
                    bRet = false;
                if (!hwDeviceOEMPA.SetEncoderWire2(1, ref digInput))
                    bRet = false;
                if (!hwDeviceOEMPA.SetExternalTriggerCycle(ref digInput))
                    bRet = false;
                if (!hwDeviceOEMPA.SetExternalTriggerSequence(ref digInput))
                    bRet = false;
                if (!hwDeviceOEMPA.Flush())
                    bRet = false;
                //set the trigger
                if (comboBoxSynchronisation.SelectedIndex != 0)
                {
                    if (!UpdateResolution())
                        bRet = false;
                }
		        if(!hwDeviceOEMPA.SetTriggerMode(ref eTrig))//trigger
			        bRet = false;
                if (!hwDeviceOEMPA.SetRequestIO(ref eReqIO))//stream IO are also required to set the trigger properly.
                    bRet = false;
                eEncoder1Type = eEncoder2Type = csEnumEncoderType.csEncoderQuadrature;
                switch (comboBoxSynchronisation.SelectedIndex)
                {
                    case 1:
                    case 2:
                        if(!hwDeviceOEMPA.SetEncoderType(ref eEncoder1Type, ref eEncoder2Type))
                            bRet = false;
                        //set wires for two encoders
                        digInput = csEnumDigitalInput.csDigitalInput01;
                        if (!hwDeviceOEMPA.SetEncoderWire1(0, ref digInput))//it is required to define wire 1 of the encoder 1
                            bRet = false;
                        digInput = csEnumDigitalInput.csDigitalInput02;
                        if (!hwDeviceOEMPA.SetEncoderWire2(0, ref digInput))//it is required to define wire 2 of the encoder 1
                            bRet = false;
                        digInput = csEnumDigitalInput.csDigitalInput03;
                        if (!hwDeviceOEMPA.SetEncoderWire1(1, ref digInput))//it is required to define wire 1 of the encoder 2
                            bRet = false;
                        digInput = csEnumDigitalInput.csDigitalInput04;
                        if (!hwDeviceOEMPA.SetEncoderWire2(1, ref digInput))//it is required to define wire 2 of the encoder 2
                            bRet = false;
                        break;
                    case 3:
                        if(!hwDeviceOEMPA.SetEncoderType(ref eEncoder1Type, ref eEncoder2Type))
                            bRet = false;
                        //set wires for the first encoder
                        digInput = csEnumDigitalInput.csDigitalInput01;
                        if (!hwDeviceOEMPA.SetEncoderWire1(0, ref digInput))//it is required to define wire 1 of the encoder 1
                            bRet = false;
                        digInput = csEnumDigitalInput.csDigitalInput02;
                        if (!hwDeviceOEMPA.SetEncoderWire2(0, ref digInput))//it is required to define wire 2 of the encoder 1
                            bRet = false;
                        //set the external sequence
                        digInput = csEnumDigitalInput.csDigitalInput04;
                        if (!hwDeviceOEMPA.SetExternalTriggerSequence(ref digInput))
                            bRet = false;
                        break;
                }
                if (!hwDeviceOEMPA.UnlockDevice())
			        bRet = false;
	        }else
		        bRet = false;
	        UpdateDialog();
	        if(!bRet)
                MessageBox.Show("Communication error!");
	        m_iComboSynchronisation = comboBoxSynchronisation.SelectedIndex;
        }

        private void buttonWriteResolution_Click(object sender, EventArgs e)
        {
            bool bRet=UpdateResolution();
            if (!bRet)
                MessageBox.Show("Communication error!");
        }
        private bool UpdateResolution()
        {
            bool bRet = true;
            Int32 lResolutionX;
            double dAux;

            if (m_bCallback)
                return false;
            //lResolutionX = Convert.ToInt32(strAux);
            if (!ConvertToDouble(textBoxResolution.Text, 1.0, out dAux))
                return false;
            lResolutionX = iround(dAux);
            textBoxResolution.Text = String.Format("{0} step/mm", lResolutionX);
            //Lock is not required with SW device.
            if (!hwDeviceOEMPA.GetSWDevice().GetSWEncoder(0).lSetResolution(lResolutionX))
		        bRet = false;
            return bRet;
        }

        private void buttonWriteStep_Click(object sender, EventArgs e)
        {
	        bool bRet=false;
	        double dAcqStep,dAux;

            if (m_bCallback)
                return;
            if(ConvertToDouble(textBoxStep.Text, 1.0e-3, out dAcqStep))
            {
                //textBoxStep.Text = String.Format("{0:F1} mm", dAcqStep/1000.0);
                dAux = dAcqStep * 1000.0;
                textBoxStep.Text = dAux.ToString(CultureInfo.InvariantCulture) + " mm";
                bRet = true;
            }
            if (!bRet)
            {
                MessageBox.Show("Error to convert string to double.");
                return;
            }
            if (hwDeviceOEMPA.LockDevice())
	        {
		        if(!hwDeviceOEMPA.SetTriggerEncoderStep(ref dAcqStep))
			        bRet = false;
                dAcqStep = dAcqStep * 1000.0;//unit millimeter
                textBoxStep.Text = String.Format("{0:F1} mm", dAcqStep);
		        if(!hwDeviceOEMPA.UnlockDevice())
			        bRet = false;
	        }else
		        bRet = false;
	        if(!bRet)
		        MessageBox.Show("Communication error!");
        }

        private void buttonResetEncoder_Click(object sender, EventArgs e)
        {
	        bool bRet=true;
	
	        if(hwDeviceOEMPA.LockDevice())
	        {
                if (!hwDeviceOEMPA.ResetTrackingTable())
                    bRet = false;
                if (!hwDeviceOEMPA.ResetEncoder(0))
			        bRet = false;
		        if(!hwDeviceOEMPA.UnlockDevice())
			        bRet = false;
	        }else
		        bRet = false;
	        if(!bRet)
		        MessageBox.Show("Communication error!");
        }

        private void checkBoxWizardLinear_CheckedChanged(object sender, EventArgs e)
        {
            if (bProcessConnection)
                return;
            File.Delete(wizardFileName);
        }

        private void buttonTemplate_Click(object sender, EventArgs e)
        {
            IniFile ini = new IniFile(wizardFileName);
            DialogResult dialogResult;
            int iErrorChannelProbe, iErrorChannelScan;

            if (wizardTemplate == null)
            {
                wizardTemplate = new csWizardTemplate();
                if (wizardTemplate == null)
                    return;
            }
            wizardTemplate.Scan.Linear = checkBoxWizardLinear.Checked;
            if (!File.Exists(wizardFileName))
                ini.TemplateWrite(wizardTemplate);
            wizardTemplate.TemplateEdit(wizardFileName, true);
            dialogResult = MessageBox.Show("Do you want to complete ?", "Wizard", MessageBoxButtons.YesNo);
            unsafe
            {
                if (dialogResult == DialogResult.Yes)
                {
                    iErrorChannelProbe = iErrorChannelScan = 0;
                    if (!ini.TemplateRead(ref wizardTemplate))
                    {
                        MessageBox.Show("Error to load template!");
                        return;
                    }
                    //You can change what you want here, for example uncomment following lines:
                    //wizardTemplate.Specimen.Velocity = 6302.0;
                    //wizardTemplate.Probe.Frequency = 5100000.0;
                    if (!wizardTemplate.TemplateToWizard())
                        MessageBox.Show("Error to save wizard from template!");
                    else if (!wizardTemplate.WizardUpdateScan(&iErrorChannelProbe, &iErrorChannelScan))
                        MessageBox.Show("Error to save wizard from template!");
                    else if ((iErrorChannelProbe > 0) || (iErrorChannelScan > 0))
                    {
                        MessageBox.Show("Error to update scan!");
                    }
                    else
                        wizardCompleted = true;
                }
                else if (dialogResult == DialogResult.No)
                {
                }
            }
            UpdateDialog();
        }

        private void buttonWizardToFile_Click(object sender, EventArgs e)
        {
            string file;
            SaveFileDialog dlg=new SaveFileDialog();
            DialogResult result;

            if ((wizardTemplate == null) || !wizardCompleted)
                return;
            //dlg.InitialDirectory = @"C:\";
            dlg.Title = "Save OEMPA Files";
            //dlg.CheckFileExists = true;
            dlg.CheckPathExists = true;
            dlg.DefaultExt = "txt";
            dlg.Filter = "Binary files (*.bin)|*.bin|Text files (*.txt)|*.txt|All files (*.*)|*.*";
            dlg.FilterIndex = 2;
            //dlg.RestoreDirectory = true;     
            if (wizardTemplate == null)
                return;
	        result = dlg.ShowDialog();
	        if (result == DialogResult.OK)
	        {
		        file = dlg.FileName;
                File.Delete(file);
                if (!wizardTemplate.WizardToFile(hwDeviceOEMPA, file))
                    MessageBox.Show("Error to save the file!");
                else
                    wizardTemplate.EditFile(file, false);
            }
        }

        private void buttonWizardToHw_Click(object sender, EventArgs e)
        {
            if ((wizardTemplate == null) || !wizardCompleted)
                return;
            if (!wizardTemplate.WizardToHw(hwDeviceOEMPA))
                MessageBox.Show("Error to update the device!");
        }

        private void Status_Click(object sender, EventArgs e)
        {
            String strFileName, line, aux, strUsb3;
            SaveFileDialog dlg = new SaveFileDialog();
            ushort wTemperature;
            ushort[,] awTemperature;
            int iSensorCountMax = 0, iSensorCount = 0, iBoardCount = 0;
            uint ulDigitalInput=0;
            bool bRet = true,bIO=false;
            csVersion version;
            csOptionsCom com;
            csOptionsTCP tcp;
            csOptionsFlash flash;
            char verChar;

            if(!hwDeviceOEMPA.GetSWDeviceOEMPA().GetTemperatureCount(out iBoardCount,out iSensorCountMax))
                return;
            awTemperature = new ushort[iBoardCount,iSensorCountMax];
            if (awTemperature == null)
                return;

            //dlg.InitialDirectory = "c:\\";
            dlg.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            dlg.FilterIndex = 1;
            //dlg.RestoreDirectory = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                strFileName = dlg.FileName;
                unsafe
                {
                    fixed (ushort* pwTemperature = awTemperature)
                    {
                        if (hwDeviceOEMPA.LockDevice())
                        {
                            ulDigitalInput = 0;
                            bIO = hwDeviceOEMPA.GetDigitalInput(&ulDigitalInput);
                            for (int iIndexBoard = 0; iIndexBoard < iBoardCount; iIndexBoard++)
                            {
                                if (!hwDeviceOEMPA.GetSWDeviceOEMPA().GetTemperatureSensorCount(iIndexBoard, out iSensorCount))
                                    return;
                                for (int iIndexSensor = 0; iIndexSensor < iSensorCount; iIndexSensor++)
                                {
                                    pwTemperature[iIndexBoard * iSensorCountMax + iIndexSensor] = 0xffff;
                                    if (!hwDeviceOEMPA.GetTemperatureSensor(iIndexBoard, iIndexSensor, &pwTemperature[iIndexBoard * iSensorCount + iIndexSensor]))
                                        bRet = false;//error
                                }
                            }
                            if (!hwDeviceOEMPA.UnlockDevice())
                                bRet = false;
                        }
                        else
                            bRet = false;
                    }
                }
                if (!bRet)
                {
                    MessageBox.Show("Communication error!");
                    return;
                }
                using (System.IO.StreamWriter file = new System.IO.StreamWriter(strFileName))
                {
                    csKernelDriver.GetVersion(out aux);
                    verChar = (char)csKernelDriver.GetVersionLetter();
                    aux += verChar;
                    line = String.Format("====SOFTWARE====\r\nDriverVersion={0}\r\n====HARDWARE====",aux);
                    file.WriteLine(line);
                    if (hwDeviceOEMPA.GetSWDeviceOEMPA().GetFWUSB3Version() > 0)
                        strUsb3 = String.Format("FWUSB3=0x{0,8:X}\r\n", hwDeviceOEMPA.GetSWDeviceOEMPA().GetFWUSB3Version());
                    else
                        strUsb3 = "";
                    line = String.Format("SystemType={0}:{1}\r\nIOBoard={2}\r\nEncoderDecimal={3}\r\nFWId = 0x{4,4:X}\r\n{5}PulserPowerMax={6}\r\nPulserPowerCurrent={7}",
                        hwDeviceOEMPA.GetSWDeviceOEMPA().GetApertureCountMax(),
                        hwDeviceOEMPA.GetSWDeviceOEMPA().GetElementCountMax(),
                        hwDeviceOEMPA.GetSWDeviceOEMPA().IsIOBoardEnabled(),
                        hwDeviceOEMPA.GetSWDeviceOEMPA().IsEncoderDecimal(),
                        hwDeviceOEMPA.GetSWDeviceOEMPA().GetFirmwareId(),
                        strUsb3,
                        hwDeviceOEMPA.GetSWDeviceOEMPA().GetPulserPowerMax(),
                        hwDeviceOEMPA.GetSWDeviceOEMPA().GetPulserPowerCurrent()
                        );
                    file.WriteLine(line);
                    hwDeviceOEMPA.GetSWDeviceOEMPA().GetSerialNumber(out aux);
                    line = String.Format("SerialNumber={0}", aux);
                    file.WriteLine(line);
                    hwDeviceOEMPA.GetSWDeviceOEMPA().GetEmbeddedVersion(out version);
                    line = String.Format("EmbeddedVersion={0}.{1}.{2}.{3}", version.uMajorMSB, version.uMajorLSB, version.uMinorMSB, version.uMinorLSB);
                    file.WriteLine(line);
                    hwDeviceOEMPA.GetSWDeviceOEMPA().GetOptionsCom(out com);
                    line = String.Format("EthernetSpeed={0}", com.uEthernetSpeed);
                    file.WriteLine(line);
                    hwDeviceOEMPA.GetSWDeviceOEMPA().GetOptionsTCP(out tcp);
                    line = String.Format("TCP={0}.{1}.{2}.{3}", tcp.uOption, tcp.uMss, tcp.uWndSize, tcp.uScale);
                    file.WriteLine(line);
                    hwDeviceOEMPA.GetSWDeviceOEMPA().GetOptionsFlash(out flash);
                    line = String.Format("flash => Manufacturer=0x{0:X2} memType={1:X2} memCapacity={2:X2}", flash.uManufacturer, flash.uMemoryType, flash.uMemoryCapacity);
                    file.WriteLine(line);
                    line = String.Format("TimeOffsetCorrectionSupported={0}", hwDeviceOEMPA.GetSWDeviceOEMPA().IsTimeOffsetSupported());
                    file.WriteLine(line);
                    if (bIO)
                    {
                        line = String.Format("DigitalInputs = 0x{0,2:X}\r\n", ulDigitalInput);
                        file.WriteLine(line);
                    }
                    line = "\nBoard\tSensor\tTemperature\n";
			        file.WriteLine(line);
                    for (int iIndexBoard = 0; iIndexBoard < iBoardCount; iIndexBoard++)
                    {
                        if (!hwDeviceOEMPA.GetSWDeviceOEMPA().GetTemperatureSensorCount(iIndexBoard, out iSensorCount))
                            return;
                        for (int iIndexSensor = 0; iIndexSensor < iSensorCount; iIndexSensor++)
                        {
                            wTemperature = awTemperature[iIndexBoard, iIndexSensor];
                            if (wTemperature == 0xffff)
                                continue;
                            line = String.Format("{0}\t{1}\t{2} °C", iIndexBoard, iIndexSensor, wTemperature);
                            file.WriteLine(line);
                        }
                    }
	                for(int iGate=0;iGate<4;iGate++)
	                {
		                line = String.Format("\n\nGate {0} (cycle Amp Tof)\n",iGate);
		                file.WriteLine(line);
		                for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
		                {
			                if(m_bAcquisitionCscanAmp!=null && m_bAcquisitionCscanAmp[iGate+4*iCycle])
			                {
				                if(m_bAcquisitionCscanAmp!=null && m_bAcquisitionCscanTof[iGate+4*iCycle])
				                {
					                line = String.Format("\t{0}\t{1}\t{2}\n",iCycle,m_sAcquisitionCscanAmp[iGate+4*iCycle],m_sAcquisitionCscanTof[iGate+4*iCycle]);
            		                file.WriteLine(line);
				                }else{
					                line = String.Format("\t{0}\t{1}\n",iCycle,m_sAcquisitionCscanAmp[iGate+4*iCycle]);
            		                file.WriteLine(line);
				                }
			                }
		                }
	                }
                    file.Close();
                }
            }
        }

        private void buttonMsgBox_Click(object sender, EventArgs e)
        {
            if (msgBox == null)
                msgBox = new FormMsgBox(hwDeviceOEMPA);
            if(msgBox!=null)
                msgBox.Show();
        }

        public bool ConvertToDouble(string strAux, double dFactor, out double dAux)
        {
            bool bRet = true;

            dAux = 0.0;
            //remove the unit
            if (strAux.IndexOf(' ') >= 0)
            {
                strAux = strAux.Substring(0,strAux.IndexOf(' '));
            }
            strAux = Number(strAux);
            try
            {
                dAux = Convert.ToDouble(strAux, CultureInfo.InvariantCulture) * dFactor;
            }
            catch (Exception ex)
            {
                Unreferenced.Parameter(ex);
                bRet = false;
            }
            return bRet;
        }

        private bool ConvertToDouble(TextBox textBox, double dFactor, out double dAux)
        {
            String strAux = textBox.Text;
            return ConvertToDouble(strAux, dFactor, out dAux);
        }

        private String Number(String strIn)
        {
            String strAux;
            int iIndex;

            //iIndex = strIn.IndexOf(" ");
            //if (iIndex < 0)
            //    return strIn;
            strAux = strIn;//.Substring(0, iIndex);
            iIndex = strAux.IndexOf(",");
            if (iIndex < 0)
                return strAux;
            strAux = strAux.Replace(',', '.');
            return strAux;
        }

        public int iround(double x)
        {
            int nx = (int)x;
            if (x > 0)
            {
                if ((x - nx) > 0.5)
                    return nx + 1;
                else
                    return nx;
            }
            else
            {
                if ((x - nx) < -0.5)
                    return nx - 1;
                else
                    return nx;
            }
        }

        private void buttonNewDevice_Click(object sender, EventArgs e)
        {
            if (nextDevice == null)
            {
                nextDevice = new OEMPAFormExample(this);
                nextDevice.Show();
                UpdateDialog();
            }
        }

        private void checkBoxMaster_CheckedChanged(object sender, EventArgs e)
        {
            OEMPAFormExample dlg;
            bool bError = false;

            if (checkBoxMaster.Checked)
            {
                dlg = firstDialog;
                while (dlg.nextDevice != null)
                {
                    if (!dlg.nextDevice.hwDeviceOEMPA.SlaveConnect(m_iHWDeviceId))
                        bError = true;
                    dlg = dlg.nextDevice;
                }
                if (bError)
                    MessageBox.Show("Error to enable the master");
            }
            else
            {
                dlg = firstDialog;
                while (dlg.nextDevice != null)
                {
                    if (dlg.nextDevice.hwDeviceOEMPA.GetHardwareLink() == csEnumHardwareLink.csSlave)
                    {
                        if (!dlg.nextDevice.hwDeviceOEMPA.SlaveDisconnect())
                            bError = true;
                    }
                    dlg = dlg.nextDevice;
                }
                if (bError)
                    MessageBox.Show("Error to disable the master");
            }
        }

        private bool buttonWrite_MasterClick()
        {
            OEMPAFormExample dlg;
            bool bError = false;
            double[] adGain;
            double[] adStart;
            double[] adRange;
            int iCount = 0,iIndex;
            bool bRet=true;

            dlg = firstDialog;
            while (dlg != null)
            {
                iCount++;
                dlg = dlg.nextDevice;
            }

            adGain = new double[iCount];
            adStart = new double[iCount];
            adRange = new double[iCount];

            iIndex = 0;
            dlg = firstDialog;
            while (dlg != null)
            {
                if (!dlg.GetGainStartRange(out adGain[iIndex], out adStart[iIndex], out adRange[iIndex]))
                {
                    MessageBox.Show("Error GetGainStartRange");
                    return false;
                }
                iIndex++;
                dlg = dlg.nextDevice;
            }
            if (bError)
                return false;
            //use Lock/Unlock for the master device only
            if (hwDeviceOEMPA.LockDevice())
            {
                iIndex = 0;
                dlg = firstDialog;
                while (dlg != null)
                {
                    bRet = dlg.SetGainStartRange(adGain[iIndex], adStart[iIndex], adRange[iIndex]);
                    if (!bRet)
                    {
                        MessageBox.Show("Error SetGainStartRange");
                        break;
                    }
                    iIndex++;
                    dlg = dlg.nextDevice;
                }
                if (!hwDeviceOEMPA.UnlockDevice())
                    bRet = false;
            }
            else
                bRet = false;
            if (!bRet)
                MessageBox.Show("Communication error!");
            return false;
        }

        private void radioButtonUSB3_CheckedChanged(object sender, EventArgs e)
        {
            if (m_bCallback)
                return;
            hwDeviceOEMPA.GetSWDeviceOEMPA().EnableUSB3(true);
            UpdateDialog();
        }

        private void radioButtonEthernet_CheckedChanged(object sender, EventArgs e)
        {
            if (m_bCallback)
                return;
            hwDeviceOEMPA.GetSWDeviceOEMPA().EnableUSB3(false);
            UpdateDialog();
        }

        private csHWDeviceOEMPA hardwareDeviceOempa;
        private void Test4()
        {
            hardwareDeviceOempa = hwDeviceOEMPA;
            double readTimeSlot = 0.05;
            int cycleIndex = 0;
            int gateId = 0;
            bool bRet = true;
            csEnumRectification rectification;

            unsafe
            {
                if (hwDeviceOEMPA.LockDevice())
                {
                    rectification = csEnumRectification.csSigned;
                    if (!this.hardwareDeviceOempa.SetAscanRectification(cycleIndex, ref rectification))
                    {
                        // throw new Fw.Contracts.Device.DeviceFailedConfigurationException("Set AscanRectification");
                        bRet = false;
                    }
                    if (!this.hardwareDeviceOempa.GetTimeSlot(cycleIndex, &readTimeSlot))
                    {
                        //throw new DeviceFailedConfigurationException("TimeSlot");
                        bRet = false;
                    }
                    csEnumBitSize readBitSize = csEnumBitSize.cs8Bits;// this.GetAosBitSize();
                    if (!this.hardwareDeviceOempa.GetAscanBitSize(&readBitSize))
                    {
                        //throw new DeviceFailedConfigurationException("AscanBitSize");
                        bRet = false;
                    }
                    //this.parameters.SetAsEnum(InspectionDeviceParameters.BitSize, readBitSize);
                    bool readEnabled = false;// this.parameters.GetAsBool(enabledParameter);
                    csEnumGateModeAmp readGateModeAmp = csEnumGateModeAmp.csAmpAbsolute;
                    csEnumGateModeTof readGateModeTof = csEnumGateModeTof.csTofAmplitudeDetection;
                    csEnumRectification readRectification = csEnumRectification.csSigned;// this.GetAosRectification();
                    double readTreshold = 0.0;// this.parameters.GetAsDouble(tresholdParameter);
                    if (!this.hardwareDeviceOempa.GetGateModeThreshold(
                        cycleIndex,
                        gateId,
                        &readEnabled,
                        &readGateModeAmp,
                        &readGateModeTof,
                        &readRectification,
                        &readTreshold))
                    {
                        //if (this.parameters.GetAsBool(enabledParameter))
                        //{
                        //    throw new DeviceFailedConfigurationException("GateModeThreshold");
                        //}
                        bRet = false;
                    }
                    if (!hwDeviceOEMPA.UnlockDevice())
                        bRet = false;
                }
                else
                    bRet = false;
                if (!bRet)
                    MessageBox.Show("Error");
                else
                    MessageBox.Show("OK");
            }
        }

        private void Test9()
        {
            hardwareDeviceOempa = hwDeviceOEMPA;
            csCustomizedAPI api;
            csCycle[] cycle;
            csFocalLaw[] focalLaw;
            csRoot root = new csRoot();
            bool bRet = true;

            api = hwDeviceOEMPA.GetCustomizedAPI();
//[Root]
//VersionDriverOEMPA=1.1.5.4
//CycleCount=1
            root.iCycleCount = 1;

//[Cycle:0]
//GainDigital=20.000000 dB
//BeamCorrection=0.000000 dB
//Start=0.000000 us
//Range=30.000000 us
//TimeSlot=2000.000000 us
//PointCount=0
//CompressionType=Compression
//Rectification=Signed
//FilterIndex=1
//GainAnalog=20.000000 dB
//GateCount=0
            cycle = new csCycle[1];
            cycle[0] = new csCycle();
            cycle[0].dGainDigital = 60.0;
            cycle[0].fBeamCorrection = 0.0f;
            cycle[0].dStart = 0.0;
            cycle[0].dRange = 10.0e-6;
            cycle[0].dTimeSlot = 0.002;
            cycle[0].lPointCount = 0;
            cycle[0].eCompressionType = csEnumCompressionType.csCompression;

//[Cycle:0\Pulser]
//WedgeDelay=0.000000 us
//CenterDelay=0.000000 us
//Element.count=1
//Element=0
//Delay.count=1;1
//Delay=0.000000 us
//Width.count=1
//Width=0.500000 us
            focalLaw = new csFocalLaw[1];
            focalLaw[0] = new csFocalLaw();
            focalLaw[0].adwElement = new UInt32[4];
            focalLaw[0].adwElement[0] = 1;
            focalLaw[0].afDelay = new float[1, 1];
            focalLaw[0].afDelay[0,0] = 0.0f;
            focalLaw[0].afPrm = new float[1];
            focalLaw[0].afPrm[0] = 0.0f;
            focalLaw[0].iPrmCount = 1;
            focalLaw[0].iDelayCount = 1;
            focalLaw[0].iFocalCount = 1;
            focalLaw[0].iElementCount = 1;

//[Cycle:0\Receiver]
//WedgeDelay=0.000000 us
//CenterDelay=0.000000 us
//Element.count=1
//Element=0
//Focusing=Standard
//Delay.count=1;1
//Delay=0.000000 us
//Gain.count=1
//Gain=0.000000 dB
//FocalTimeOfFlight.count=1
//FocalTimeOfFlight=0.000000 us
            bRet = api.WriteHW(ref hwDeviceOEMPA, ref root, ref cycle, ref focalLaw, ref focalLaw, csEnumAcquisitionState.csAcqOff);
            Unreferenced.Parameter(bRet);
        }

        private void Test5()
        {
            bool bRet = true;
            int iTrackingCycleIndex,iTrackingGateIndex;
            MessageBoxButtons buttons = MessageBoxButtons.YesNo;
            DialogResult result;
            bool bEnable;

            result = MessageBox.Show("Do you want to enable the tracking?", "Warning", buttons);
            if (result == System.Windows.Forms.DialogResult.Yes)
                bEnable = true;
            else
                bEnable = false;
            if (hwDeviceOEMPA.LockDevice())
            {
                iTrackingCycleIndex = iTrackingGateIndex = 0;
                if (!hwDeviceOEMPA.SetTrackingGateStart(0, 1, bEnable, ref iTrackingCycleIndex, ref iTrackingGateIndex))
                    bRet = false;
                if (!hwDeviceOEMPA.SetTrackingGateStop(0, 1, bEnable, ref iTrackingCycleIndex, ref iTrackingGateIndex))
                    bRet = false;
                if (!hwDeviceOEMPA.UnlockDevice())
                    bRet = false;
            }
            else
                bRet = false;
            if (!bRet)
                MessageBox.Show("Error");
            else
                MessageBox.Show("OK");
        }
        unsafe private void Test7()
        {
#if _WIN64
            csAcquisitionFifo fifo;
            int iCount,iSize,iIndexTail,iIndexHead;
            csAcqInfoEx acq,acq2;
            csHeaderStream_0x0001 stream, stream2;
            csSubStreamAscan_0x0103 ascan, ascan2;
            UInt32[] tab=GetAscanSubStream();
            void *pBufferMax;
            void *pBufferMin;
            void *pBufferSat;

            acq = new csAcqInfoEx();
            stream = new csHeaderStream_0x0001();
            acq.bFullMatrixCapture = 0;
            acq.bMultiChannel = 0;
            acq.dEncoder = new double[2];
            acq.dEncoder[0] = 0.0;
            acq.dEncoder[1] = 0.0;
            acq.dwDigitalInputs = 0;
            acq.lDeviceId = 0;
            acq.lEncoder = new int[2];
            acq.lEncoder[0] = 0;
            acq.lEncoder[1] = 0;
            acq.lFMCElementIndex = 0;
            stream.frameId = 0;
            stream.settingId = 0;
            stream.size = 0;
            stream.start = 0;
            stream.subStreamCount = 0;
            stream.version = 0;
            fifo = new csAcquisitionFifo(csEnumAcquisitionFifo.csFifoAscan);
            if (!fifo.Alloc(1024, 1024 * 1024))
                return;
            iCount = fifo.GetCount();
            fixed (UInt32* p = &tab[0])
            {
                byte *pSubStreamData=(byte*)p;
                iSize = (int)p[0] >> 16;
                iSize += sizeof(ulong);
                iSize += 0xBB9 / 4;
                stream.subStreamCount = 1;
                if (fifo.InFifo(pSubStreamData, iSize, ref acq, ref stream))
                {
                    iCount = fifo.GetCount();
                }
                stream.subStreamCount = 2;
                if (fifo.InFifo(pSubStreamData, iSize, ref acq, ref stream))
                {
                    iCount = fifo.GetCount();
                }
                iCount = fifo.GetItemLimit(out iIndexTail, out iIndexHead);
                if (fifo.OutAscan(iIndexTail, true, out acq2, out stream2, out ascan2, out pBufferMax, out pBufferMin, out pBufferSat))
                {
                    fifo.IncrementItemIndex(ref iIndexTail);
                    fifo.RemoveTail();
                }
                iCount = fifo.GetItemLimit(out iIndexTail, out iIndexHead);
                if (fifo.OutAscan(iIndexTail, true, out acq2, out stream2, out ascan2, out pBufferMax, out pBufferMin, out pBufferSat))
                {
                    fifo.IncrementItemIndex(ref iIndexTail);
                    fifo.RemoveTail();
                }
            }
            fifo.Dispose();
#endif
        }
        private void Test6()
        {
            bool bRet = true;
            acsDac dac;
            double dTime;
            csEnumBitSize eBitSize;
            acsByte[] abyData;

            abyData = new acsByte[15];
            unsafe
            {
                if (hwDeviceOEMPA.LockDevice())
                {
                    for (int iCycle = 0; iCycle < 15; iCycle++)
                    {
                        if (!hwDeviceOEMPA.GetMultiHWChannelAcqDecimation(iCycle, out abyData[iCycle]))
                            bRet = false;
                    }
                    if (!hwDeviceOEMPA.GetDACGain(0, out dac))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetTimeSlot(0, &dTime))
                        bRet = false;
                    if (!hwDeviceOEMPA.GetAscanBitSize(&eBitSize))
                        bRet = false;
                    if (!hwDeviceOEMPA.UnlockDevice())
                        bRet = false;
                }
                else
                    bRet = false;
            }
            if (!bRet)
                MessageBox.Show("Error");
            else
                MessageBox.Show("OK");
        }

        private void Test8()
        {
            bool bRet = true;
            double[] adGain;

            adGain = new double[15];
            unsafe
            {
                fixed (double* pGain = adGain)
                {
                    if (hwDeviceOEMPA.LockDevice())
                    {
                        for (int iCycle = 0; iCycle < 15; iCycle++)
                        {
                            if (!hwDeviceOEMPA.GetGainDigital(iCycle, &pGain[iCycle]))
                                bRet = false;
                        }
                        if (!hwDeviceOEMPA.UnlockDevice())
                            bRet = false;
                    }
                    else
                        bRet = false;
                }
            }
            if (!bRet)
                MessageBox.Show("Error");
            else
                MessageBox.Show("OK");
        }

        private UInt32[] GetAscanSubStream()
        {
            UInt32[] pTab ={0x0BDC0103,
	                0x8AFC0C9A,
	                0x00000001,
	                0x0BB90000,
	                0x00009111,
	                0xFFFFFFFF,
	                0x0000FFFF,
	                0x00000006,
	                0x00000000,
	                0x00FFFF00,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x01010101,
	                0x01010101,
	                0x02020202,
	                0x03030302,
	                0x04030303,
	                0x04050404,
	                0x05050504,
	                0x06060605,
	                0x06070706,
	                0x07090807,
	                0x070A0A07,
	                0x070D0C07,
	                0xFF140E04,
	                0xC3B5B6D5,
	                0xD7D1CDCC,
	                0xE4DFDCDB,
	                0xEDEAE8E7,
	                0xF5F2F0EF,
	                0xFAF8F7F6,
	                0xFDFCFBFB,
	                0x00FFFEFE,
	                0x01010100,
	                0x02020202,
	                0x03030303,
	                0x03030303,
	                0x03030303,
	                0x03030303,
	                0x02020203,
	                0x02020202,
	                0x01010202,
	                0x01010101,
	                0x00000001,
	                0x00000000,
	                0xFFFF0000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFEFFFFFF,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFDFDFEFE,
	                0xFDFDFDFD,
	                0xFDFDFDFD,
	                0xFDFCFDFD,
	                0xFEFCFCFD,
	                0x0EFEFCFE,
	                0x47473D27,
	                0x1B2C3A43,
	                0xDCE5F609,
	                0xE5E5E2DD,
	                0xEBEBE9E7,
	                0xF0EFEEED,
	                0xF4F3F2F1,
	                0x0805FFF8,
	                0x0A0A0A0A,
	                0x0C0C0C0B,
	                0x0B0C0C0C,
	                0x090A0B0B,
	                0xFF020507,
	                0xF2F4F7FB,
	                0xF0EFEFF0,
	                0xF8F5F2F1,
	                0x0300FDFA,
	                0x07070604,
	                0x05060707,
	                0xFF010204,
	                0xFBFBFCFE,
	                0xFAFAFBFB,
	                0xFAFBFBFB,
	                0xF9FAFAFA,
	                0xF9F9F8F9,
	                0xFDFCFAF9,
	                0x050301FF,
	                0x0C0B0907,
	                0x0E0E0E0D,
	                0x0A0C0D0E,
	                0x04050709,
	                0x00010203,
	                0x00000000,
	                0x02010100,
	                0x03030202,
	                0x05040403,
	                0x06060505,
	                0x07070707,
	                0x07070707,
	                0x05060607,
	                0x02020304,
	                0xFEFF0001,
	                0xFDFDFDFE,
	                0xFDFDFDFD,
	                0xFDFDFDFD,
	                0xFCFCFCFD,
	                0xFCFCFCFC,
	                0xFCFCFCFC,
	                0xFCFCFCFC,
	                0xFDFDFDFD,
	                0xFFFFFEFE,
	                0xFFFFFFFF,
	                0x00000000,
	                0x01010101,
	                0x02020201,
	                0x02020202,
	                0x03020202,
	                0x02020202,
	                0x01010202,
	                0x00010101,
	                0x00000000,
	                0xFFFFFF00,
	                0xFEFFFFFF,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFFFEFEFE,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0xFF0000FF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x01000000,
	                0x01010101,
	                0x01010101,
	                0x01010101,
	                0x02010101,
	                0x01010101,
	                0x01010101,
	                0x01010101,
	                0x01010101,
	                0x01010101,
	                0x00010101,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFEFFFFFF,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFFFEFEFE,
	                0xFFFFFEFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00000000,
	                0x00000000,
	                0x000000FF,
	                0xFFFFFF00,
	                0xFF0000FF,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0x00FFFF00,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x0000FF00,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF00FFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00FFFF00,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00FFFF00,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0x000000FF,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF00FFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00000000,
	                0xFF00FF00,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0x00FFFF00,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00FFFF00,
	                0xFF000000,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFF0000,
	                0xFFFFFFFF,
	                0xFEFFFFFF,
	                0xFEFEFEFE,
	                0xFEFEFEFE,
	                0xFFFFFFFE,
	                0xFFFFFFFF,
	                0x0000FFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFF0000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF000000,
	                0x000000FF,
	                0x00FFFF00,
	                0x00000000,
	                0xFFFF0000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFF0000,
	                0xFEFFFFFF,
	                0xFFFEFEFE,
	                0xFFFFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFF0000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00FF0000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0x0000FFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0xFF0000FF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x0000FFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0x00000000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00FFFF00,
	                0xFF000000,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0x00FFFFFF,
	                0x00000000,
	                0x00FF0000,
	                0x00000000,
	                0xFF000000,
	                0x000000FF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0x000000FF,
	                0x00FFFF00,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0xFF00FF00,
	                0x000000FF,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0x000000FF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x0000FFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF00FF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00FFFF00,
	                0x00FFFF00,
	                0xFFFF0000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0x000000FF,
	                0x00FFFF00,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFF0000FF,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0x00FFFFFF,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFF00FF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0xFF000000,
	                0x000000FF,
	                0x00FFFF00,
	                0xFFFFFF00,
	                0x000000FF,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFF0000FF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFF00,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00FFFF00,
	                0x00000000,
	                0x00FF0000,
	                0xFF000000,
	                0xFF0000FF,
	                0xFFFFFFFF,
	                0x000000FF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFF000000,
	                0xFFFFFF00,
	                0x000000FF,
	                0xFF00FF00,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0xFFFFFFFF,
	                0x00FFFFFF,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0x00000000,
	                0xFFFFFF00,
	                0x0000FFFF};

            return pTab;
        }
    }

    public class Unreferenced
    {
        [System.Diagnostics.Conditional("DEBUG")]
        static public void Parameter(params object[] o)
        {
            return;
        }
    }
}
