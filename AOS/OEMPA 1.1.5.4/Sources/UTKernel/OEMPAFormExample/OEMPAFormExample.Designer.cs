namespace OEMPAFormExample
{
    partial class OEMPAFormExample
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.checkBoxConnect = new System.Windows.Forms.CheckBox();
            this.textBoxIPAddress = new System.Windows.Forms.TextBox();
            this.textBoxStatus = new System.Windows.Forms.TextBox();
            this.buttonRead = new System.Windows.Forms.Button();
            this.textBoxRead = new System.Windows.Forms.TextBox();
            this.buttonWrite = new System.Windows.Forms.Button();
            this.textBoxWriteGain = new System.Windows.Forms.TextBox();
            this.checkBoxPulser = new System.Windows.Forms.CheckBox();
            this.buttonLoad = new System.Windows.Forms.Button();
            this.textBoxFileName = new System.Windows.Forms.TextBox();
            this.textBoxFileStatus = new System.Windows.Forms.TextBox();
            this.comboBoxSynchronisation = new System.Windows.Forms.ComboBox();
            this.textBoxResolution = new System.Windows.Forms.TextBox();
            this.labelResolution = new System.Windows.Forms.Label();
            this.labelStep = new System.Windows.Forms.Label();
            this.textBoxStep = new System.Windows.Forms.TextBox();
            this.buttonResetEncoder = new System.Windows.Forms.Button();
            this.buttonWriteResolution = new System.Windows.Forms.Button();
            this.buttonWriteStep = new System.Windows.Forms.Button();
            this.groupBoxSynchronisation = new System.Windows.Forms.GroupBox();
            this.groupBoxLowLevelAPI = new System.Windows.Forms.GroupBox();
            this.groupBoxUSB3 = new System.Windows.Forms.GroupBox();
            this.radioButtonEthernet = new System.Windows.Forms.RadioButton();
            this.radioButtonUSB3 = new System.Windows.Forms.RadioButton();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonMsgBox = new System.Windows.Forms.Button();
            this.buttonStatus = new System.Windows.Forms.Button();
            this.labelDataLost = new System.Windows.Forms.Label();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.labelIP = new System.Windows.Forms.Label();
            this.buttonTest = new System.Windows.Forms.Button();
            this.groupBoxMediumLevelAPI = new System.Windows.Forms.GroupBox();
            this.buttonSave = new System.Windows.Forms.Button();
            this.groupBoxWizard = new System.Windows.Forms.GroupBox();
            this.checkBoxWizardLinear = new System.Windows.Forms.CheckBox();
            this.buttonWizardToHw = new System.Windows.Forms.Button();
            this.buttonWizardToFile = new System.Windows.Forms.Button();
            this.buttonTemplate = new System.Windows.Forms.Button();
            this.textBoxWriteStart = new System.Windows.Forms.TextBox();
            this.textBoxWriteRange = new System.Windows.Forms.TextBox();
            this.buttonNewDevice = new System.Windows.Forms.Button();
            this.checkBoxMaster = new System.Windows.Forms.CheckBox();
            this.groupBoxLowLevelAPI.SuspendLayout();
            this.groupBoxUSB3.SuspendLayout();
            this.groupBoxMediumLevelAPI.SuspendLayout();
            this.groupBoxWizard.SuspendLayout();
            this.SuspendLayout();
            // 
            // checkBoxConnect
            // 
            this.checkBoxConnect.AutoSize = true;
            this.checkBoxConnect.Location = new System.Drawing.Point(290, 36);
            this.checkBoxConnect.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.checkBoxConnect.Name = "checkBoxConnect";
            this.checkBoxConnect.Size = new System.Drawing.Size(95, 24);
            this.checkBoxConnect.TabIndex = 0;
            this.checkBoxConnect.Text = "Connect";
            this.checkBoxConnect.UseVisualStyleBackColor = true;
            this.checkBoxConnect.CheckedChanged += new System.EventHandler(this.checkBoxConnect_CheckedChanged);
            // 
            // textBoxIPAddress
            // 
            this.textBoxIPAddress.Location = new System.Drawing.Point(53, 39);
            this.textBoxIPAddress.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxIPAddress.Name = "textBoxIPAddress";
            this.textBoxIPAddress.Size = new System.Drawing.Size(163, 26);
            this.textBoxIPAddress.TabIndex = 1;
            this.textBoxIPAddress.Text = "192.168.1.11";
            // 
            // textBoxStatus
            // 
            this.textBoxStatus.Location = new System.Drawing.Point(17, 74);
            this.textBoxStatus.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxStatus.Name = "textBoxStatus";
            this.textBoxStatus.Size = new System.Drawing.Size(674, 26);
            this.textBoxStatus.TabIndex = 2;
            // 
            // buttonRead
            // 
            this.buttonRead.Location = new System.Drawing.Point(17, 109);
            this.buttonRead.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonRead.Name = "buttonRead";
            this.buttonRead.Size = new System.Drawing.Size(112, 35);
            this.buttonRead.TabIndex = 3;
            this.buttonRead.Text = "Read>";
            this.buttonRead.UseVisualStyleBackColor = true;
            this.buttonRead.Click += new System.EventHandler(this.buttonRead_Click);
            // 
            // textBoxRead
            // 
            this.textBoxRead.Location = new System.Drawing.Point(138, 112);
            this.textBoxRead.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxRead.Name = "textBoxRead";
            this.textBoxRead.Size = new System.Drawing.Size(433, 26);
            this.textBoxRead.TabIndex = 4;
            // 
            // buttonWrite
            // 
            this.buttonWrite.Location = new System.Drawing.Point(17, 152);
            this.buttonWrite.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonWrite.Name = "buttonWrite";
            this.buttonWrite.Size = new System.Drawing.Size(112, 35);
            this.buttonWrite.TabIndex = 5;
            this.buttonWrite.Text = "Write>";
            this.buttonWrite.UseVisualStyleBackColor = true;
            this.buttonWrite.Click += new System.EventHandler(this.buttonWrite_Click);
            // 
            // textBoxWriteGain
            // 
            this.textBoxWriteGain.Location = new System.Drawing.Point(184, 156);
            this.textBoxWriteGain.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxWriteGain.Name = "textBoxWriteGain";
            this.textBoxWriteGain.Size = new System.Drawing.Size(72, 26);
            this.textBoxWriteGain.TabIndex = 6;
            // 
            // checkBoxPulser
            // 
            this.checkBoxPulser.AutoSize = true;
            this.checkBoxPulser.Location = new System.Drawing.Point(403, 36);
            this.checkBoxPulser.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.checkBoxPulser.Name = "checkBoxPulser";
            this.checkBoxPulser.Size = new System.Drawing.Size(79, 24);
            this.checkBoxPulser.TabIndex = 8;
            this.checkBoxPulser.Text = "Pulser";
            this.checkBoxPulser.UseVisualStyleBackColor = true;
            this.checkBoxPulser.CheckedChanged += new System.EventHandler(this.checkBoxPulser_CheckedChanged);
            // 
            // buttonLoad
            // 
            this.buttonLoad.Location = new System.Drawing.Point(17, 366);
            this.buttonLoad.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonLoad.Name = "buttonLoad";
            this.buttonLoad.Size = new System.Drawing.Size(112, 35);
            this.buttonLoad.TabIndex = 9;
            this.buttonLoad.Text = "file => hw";
            this.buttonLoad.UseVisualStyleBackColor = true;
            this.buttonLoad.Click += new System.EventHandler(this.buttonLoad_Click);
            // 
            // textBoxFileName
            // 
            this.textBoxFileName.Location = new System.Drawing.Point(10, 66);
            this.textBoxFileName.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxFileName.Name = "textBoxFileName";
            this.textBoxFileName.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.textBoxFileName.Size = new System.Drawing.Size(672, 26);
            this.textBoxFileName.TabIndex = 10;
            // 
            // textBoxFileStatus
            // 
            this.textBoxFileStatus.Location = new System.Drawing.Point(129, 32);
            this.textBoxFileStatus.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxFileStatus.Name = "textBoxFileStatus";
            this.textBoxFileStatus.Size = new System.Drawing.Size(433, 26);
            this.textBoxFileStatus.TabIndex = 11;
            // 
            // comboBoxSynchronisation
            // 
            this.comboBoxSynchronisation.FormattingEnabled = true;
            this.comboBoxSynchronisation.Items.AddRange(new object[] {
            "Internal",
            "Encoder",
            "Mixed",
            "ExternalSequence"});
            this.comboBoxSynchronisation.Location = new System.Drawing.Point(138, 230);
            this.comboBoxSynchronisation.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.comboBoxSynchronisation.Name = "comboBoxSynchronisation";
            this.comboBoxSynchronisation.Size = new System.Drawing.Size(180, 28);
            this.comboBoxSynchronisation.TabIndex = 12;
            this.comboBoxSynchronisation.SelectedIndexChanged += new System.EventHandler(this.comboBoxSynchronisation_SelectedIndexChanged);
            // 
            // textBoxResolution
            // 
            this.textBoxResolution.Location = new System.Drawing.Point(554, 231);
            this.textBoxResolution.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxResolution.Name = "textBoxResolution";
            this.textBoxResolution.Size = new System.Drawing.Size(100, 26);
            this.textBoxResolution.TabIndex = 13;
            // 
            // labelResolution
            // 
            this.labelResolution.AutoSize = true;
            this.labelResolution.Location = new System.Drawing.Point(458, 236);
            this.labelResolution.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelResolution.Name = "labelResolution";
            this.labelResolution.Size = new System.Drawing.Size(85, 20);
            this.labelResolution.TabIndex = 14;
            this.labelResolution.Text = "Resolution";
            // 
            // labelStep
            // 
            this.labelStep.AutoSize = true;
            this.labelStep.Location = new System.Drawing.Point(459, 272);
            this.labelStep.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelStep.Name = "labelStep";
            this.labelStep.Size = new System.Drawing.Size(43, 20);
            this.labelStep.TabIndex = 16;
            this.labelStep.Text = "Step";
            // 
            // textBoxStep
            // 
            this.textBoxStep.Location = new System.Drawing.Point(555, 268);
            this.textBoxStep.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxStep.Name = "textBoxStep";
            this.textBoxStep.Size = new System.Drawing.Size(100, 26);
            this.textBoxStep.TabIndex = 15;
            // 
            // buttonResetEncoder
            // 
            this.buttonResetEncoder.Location = new System.Drawing.Point(145, 264);
            this.buttonResetEncoder.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonResetEncoder.Name = "buttonResetEncoder";
            this.buttonResetEncoder.Size = new System.Drawing.Size(143, 35);
            this.buttonResetEncoder.TabIndex = 17;
            this.buttonResetEncoder.Text = "Reset Encoder";
            this.buttonResetEncoder.UseVisualStyleBackColor = true;
            this.buttonResetEncoder.Click += new System.EventHandler(this.buttonResetEncoder_Click);
            // 
            // buttonWriteResolution
            // 
            this.buttonWriteResolution.Location = new System.Drawing.Point(338, 228);
            this.buttonWriteResolution.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonWriteResolution.Name = "buttonWriteResolution";
            this.buttonWriteResolution.Size = new System.Drawing.Size(116, 35);
            this.buttonWriteResolution.TabIndex = 18;
            this.buttonWriteResolution.Text = "Write>";
            this.buttonWriteResolution.UseVisualStyleBackColor = true;
            this.buttonWriteResolution.Click += new System.EventHandler(this.buttonWriteResolution_Click);
            // 
            // buttonWriteStep
            // 
            this.buttonWriteStep.Location = new System.Drawing.Point(338, 264);
            this.buttonWriteStep.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonWriteStep.Name = "buttonWriteStep";
            this.buttonWriteStep.Size = new System.Drawing.Size(116, 35);
            this.buttonWriteStep.TabIndex = 19;
            this.buttonWriteStep.Text = "Write>";
            this.buttonWriteStep.UseVisualStyleBackColor = true;
            this.buttonWriteStep.Click += new System.EventHandler(this.buttonWriteStep_Click);
            // 
            // groupBoxSynchronisation
            // 
            this.groupBoxSynchronisation.Location = new System.Drawing.Point(126, 201);
            this.groupBoxSynchronisation.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxSynchronisation.Name = "groupBoxSynchronisation";
            this.groupBoxSynchronisation.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxSynchronisation.Size = new System.Drawing.Size(549, 108);
            this.groupBoxSynchronisation.TabIndex = 20;
            this.groupBoxSynchronisation.TabStop = false;
            this.groupBoxSynchronisation.Text = "Synchronisation";
            // 
            // groupBoxLowLevelAPI
            // 
            this.groupBoxLowLevelAPI.Controls.Add(this.groupBoxUSB3);
            this.groupBoxLowLevelAPI.Controls.Add(this.label3);
            this.groupBoxLowLevelAPI.Controls.Add(this.label2);
            this.groupBoxLowLevelAPI.Controls.Add(this.label1);
            this.groupBoxLowLevelAPI.Controls.Add(this.buttonMsgBox);
            this.groupBoxLowLevelAPI.Controls.Add(this.buttonStatus);
            this.groupBoxLowLevelAPI.Controls.Add(this.labelDataLost);
            this.groupBoxLowLevelAPI.Controls.Add(this.textBoxPort);
            this.groupBoxLowLevelAPI.Controls.Add(this.labelIP);
            this.groupBoxLowLevelAPI.Controls.Add(this.buttonTest);
            this.groupBoxLowLevelAPI.Controls.Add(this.checkBoxPulser);
            this.groupBoxLowLevelAPI.Controls.Add(this.checkBoxConnect);
            this.groupBoxLowLevelAPI.Location = new System.Drawing.Point(9, 5);
            this.groupBoxLowLevelAPI.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxLowLevelAPI.Name = "groupBoxLowLevelAPI";
            this.groupBoxLowLevelAPI.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxLowLevelAPI.Size = new System.Drawing.Size(692, 322);
            this.groupBoxLowLevelAPI.TabIndex = 21;
            this.groupBoxLowLevelAPI.TabStop = false;
            this.groupBoxLowLevelAPI.Text = "Low Level API";
            // 
            // groupBoxUSB3
            // 
            this.groupBoxUSB3.Controls.Add(this.radioButtonEthernet);
            this.groupBoxUSB3.Controls.Add(this.radioButtonUSB3);
            this.groupBoxUSB3.Location = new System.Drawing.Point(10, 204);
            this.groupBoxUSB3.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxUSB3.Name = "groupBoxUSB3";
            this.groupBoxUSB3.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxUSB3.Size = new System.Drawing.Size(84, 84);
            this.groupBoxUSB3.TabIndex = 21;
            this.groupBoxUSB3.TabStop = false;
            this.groupBoxUSB3.Text = "USB3";
            // 
            // radioButtonEthernet
            // 
            this.radioButtonEthernet.AutoSize = true;
            this.radioButtonEthernet.Location = new System.Drawing.Point(12, 49);
            this.radioButtonEthernet.Name = "radioButtonEthernet";
            this.radioButtonEthernet.Size = new System.Drawing.Size(65, 24);
            this.radioButtonEthernet.TabIndex = 36;
            this.radioButtonEthernet.TabStop = true;
            this.radioButtonEthernet.Text = "LAN";
            this.radioButtonEthernet.UseVisualStyleBackColor = true;
            this.radioButtonEthernet.CheckedChanged += new System.EventHandler(this.radioButtonEthernet_CheckedChanged);
            // 
            // radioButtonUSB3
            // 
            this.radioButtonUSB3.AutoSize = true;
            this.radioButtonUSB3.Location = new System.Drawing.Point(12, 24);
            this.radioButtonUSB3.Name = "radioButtonUSB3";
            this.radioButtonUSB3.Size = new System.Drawing.Size(68, 24);
            this.radioButtonUSB3.TabIndex = 35;
            this.radioButtonUSB3.TabStop = true;
            this.radioButtonUSB3.Text = "USB";
            this.radioButtonUSB3.UseVisualStyleBackColor = true;
            this.radioButtonUSB3.CheckedChanged += new System.EventHandler(this.radioButtonUSB3_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(371, 154);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 20);
            this.label3.TabIndex = 34;
            this.label3.Text = "Range";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(249, 154);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 20);
            this.label2.TabIndex = 32;
            this.label2.Text = "Start";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(128, 154);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(43, 20);
            this.label1.TabIndex = 30;
            this.label1.Text = "Gain";
            // 
            // buttonMsgBox
            // 
            this.buttonMsgBox.Location = new System.Drawing.Point(572, 102);
            this.buttonMsgBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonMsgBox.Name = "buttonMsgBox";
            this.buttonMsgBox.Size = new System.Drawing.Size(112, 35);
            this.buttonMsgBox.TabIndex = 28;
            this.buttonMsgBox.Text = "Msg Box";
            this.buttonMsgBox.UseVisualStyleBackColor = true;
            this.buttonMsgBox.Visible = false;
            this.buttonMsgBox.Click += new System.EventHandler(this.buttonMsgBox_Click);
            // 
            // buttonStatus
            // 
            this.buttonStatus.Location = new System.Drawing.Point(572, 148);
            this.buttonStatus.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonStatus.Name = "buttonStatus";
            this.buttonStatus.Size = new System.Drawing.Size(112, 35);
            this.buttonStatus.TabIndex = 27;
            this.buttonStatus.Text = "Status";
            this.buttonStatus.UseVisualStyleBackColor = true;
            this.buttonStatus.Visible = false;
            this.buttonStatus.Click += new System.EventHandler(this.Status_Click);
            // 
            // labelDataLost
            // 
            this.labelDataLost.AutoSize = true;
            this.labelDataLost.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F);
            this.labelDataLost.ForeColor = System.Drawing.SystemColors.ControlText;
            this.labelDataLost.Location = new System.Drawing.Point(575, 39);
            this.labelDataLost.Name = "labelDataLost";
            this.labelDataLost.Size = new System.Drawing.Size(79, 20);
            this.labelDataLost.TabIndex = 26;
            this.labelDataLost.Text = "Data Lost";
            this.labelDataLost.Visible = false;
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(216, 34);
            this.textBoxPort.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(62, 26);
            this.textBoxPort.TabIndex = 23;
            // 
            // labelIP
            // 
            this.labelIP.AutoSize = true;
            this.labelIP.Location = new System.Drawing.Point(10, 36);
            this.labelIP.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelIP.Name = "labelIP";
            this.labelIP.Size = new System.Drawing.Size(24, 20);
            this.labelIP.TabIndex = 25;
            this.labelIP.Text = "IP";
            // 
            // buttonTest
            // 
            this.buttonTest.Location = new System.Drawing.Point(514, 148);
            this.buttonTest.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonTest.Name = "buttonTest";
            this.buttonTest.Size = new System.Drawing.Size(48, 35);
            this.buttonTest.TabIndex = 24;
            this.buttonTest.Text = "test";
            this.buttonTest.UseVisualStyleBackColor = true;
            this.buttonTest.Click += new System.EventHandler(this.buttonTest_Click);
            // 
            // groupBoxMediumLevelAPI
            // 
            this.groupBoxMediumLevelAPI.Controls.Add(this.buttonSave);
            this.groupBoxMediumLevelAPI.Controls.Add(this.textBoxFileStatus);
            this.groupBoxMediumLevelAPI.Controls.Add(this.textBoxFileName);
            this.groupBoxMediumLevelAPI.Location = new System.Drawing.Point(9, 338);
            this.groupBoxMediumLevelAPI.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxMediumLevelAPI.Name = "groupBoxMediumLevelAPI";
            this.groupBoxMediumLevelAPI.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.groupBoxMediumLevelAPI.Size = new System.Drawing.Size(692, 149);
            this.groupBoxMediumLevelAPI.TabIndex = 22;
            this.groupBoxMediumLevelAPI.TabStop = false;
            this.groupBoxMediumLevelAPI.Text = "Medium Level API";
            // 
            // buttonSave
            // 
            this.buttonSave.Location = new System.Drawing.Point(9, 105);
            this.buttonSave.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(112, 35);
            this.buttonSave.TabIndex = 23;
            this.buttonSave.Text = "hw => file";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonSave_Click);
            // 
            // groupBoxWizard
            // 
            this.groupBoxWizard.Controls.Add(this.checkBoxWizardLinear);
            this.groupBoxWizard.Controls.Add(this.buttonWizardToHw);
            this.groupBoxWizard.Controls.Add(this.buttonWizardToFile);
            this.groupBoxWizard.Controls.Add(this.buttonTemplate);
            this.groupBoxWizard.Location = new System.Drawing.Point(9, 496);
            this.groupBoxWizard.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBoxWizard.Name = "groupBoxWizard";
            this.groupBoxWizard.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.groupBoxWizard.Size = new System.Drawing.Size(502, 69);
            this.groupBoxWizard.TabIndex = 23;
            this.groupBoxWizard.TabStop = false;
            this.groupBoxWizard.Text = "Wizard (Single Channel)";
            // 
            // checkBoxWizardLinear
            // 
            this.checkBoxWizardLinear.AutoSize = true;
            this.checkBoxWizardLinear.Checked = true;
            this.checkBoxWizardLinear.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxWizardLinear.Location = new System.Drawing.Point(22, 32);
            this.checkBoxWizardLinear.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.checkBoxWizardLinear.Name = "checkBoxWizardLinear";
            this.checkBoxWizardLinear.Size = new System.Drawing.Size(79, 24);
            this.checkBoxWizardLinear.TabIndex = 35;
            this.checkBoxWizardLinear.Text = "Linear";
            this.checkBoxWizardLinear.UseVisualStyleBackColor = true;
            this.checkBoxWizardLinear.CheckedChanged += new System.EventHandler(this.checkBoxWizardLinear_CheckedChanged);
            // 
            // buttonWizardToHw
            // 
            this.buttonWizardToHw.Location = new System.Drawing.Point(370, 26);
            this.buttonWizardToHw.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.buttonWizardToHw.Name = "buttonWizardToHw";
            this.buttonWizardToHw.Size = new System.Drawing.Size(112, 35);
            this.buttonWizardToHw.TabIndex = 2;
            this.buttonWizardToHw.Text = "wiz => hw";
            this.buttonWizardToHw.UseVisualStyleBackColor = true;
            this.buttonWizardToHw.Click += new System.EventHandler(this.buttonWizardToHw_Click);
            // 
            // buttonWizardToFile
            // 
            this.buttonWizardToFile.Location = new System.Drawing.Point(236, 26);
            this.buttonWizardToFile.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.buttonWizardToFile.Name = "buttonWizardToFile";
            this.buttonWizardToFile.Size = new System.Drawing.Size(112, 35);
            this.buttonWizardToFile.TabIndex = 1;
            this.buttonWizardToFile.Text = "wiz => file";
            this.buttonWizardToFile.UseVisualStyleBackColor = true;
            this.buttonWizardToFile.Click += new System.EventHandler(this.buttonWizardToFile_Click);
            // 
            // buttonTemplate
            // 
            this.buttonTemplate.Location = new System.Drawing.Point(110, 26);
            this.buttonTemplate.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.buttonTemplate.Name = "buttonTemplate";
            this.buttonTemplate.Size = new System.Drawing.Size(112, 35);
            this.buttonTemplate.TabIndex = 0;
            this.buttonTemplate.Text = "Template";
            this.buttonTemplate.UseVisualStyleBackColor = true;
            this.buttonTemplate.Click += new System.EventHandler(this.buttonTemplate_Click);
            // 
            // textBoxWriteStart
            // 
            this.textBoxWriteStart.Location = new System.Drawing.Point(306, 156);
            this.textBoxWriteStart.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxWriteStart.Name = "textBoxWriteStart";
            this.textBoxWriteStart.Size = new System.Drawing.Size(72, 26);
            this.textBoxWriteStart.TabIndex = 31;
            // 
            // textBoxWriteRange
            // 
            this.textBoxWriteRange.Location = new System.Drawing.Point(439, 156);
            this.textBoxWriteRange.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.textBoxWriteRange.Name = "textBoxWriteRange";
            this.textBoxWriteRange.Size = new System.Drawing.Size(72, 26);
            this.textBoxWriteRange.TabIndex = 33;
            // 
            // buttonNewDevice
            // 
            this.buttonNewDevice.Location = new System.Drawing.Point(515, 520);
            this.buttonNewDevice.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.buttonNewDevice.Name = "buttonNewDevice";
            this.buttonNewDevice.Size = new System.Drawing.Size(101, 35);
            this.buttonNewDevice.TabIndex = 36;
            this.buttonNewDevice.Text = "New Device";
            this.buttonNewDevice.UseVisualStyleBackColor = true;
            this.buttonNewDevice.Click += new System.EventHandler(this.buttonNewDevice_Click);
            // 
            // checkBoxMaster
            // 
            this.checkBoxMaster.AutoSize = true;
            this.checkBoxMaster.Location = new System.Drawing.Point(622, 526);
            this.checkBoxMaster.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.checkBoxMaster.Name = "checkBoxMaster";
            this.checkBoxMaster.Size = new System.Drawing.Size(84, 24);
            this.checkBoxMaster.TabIndex = 35;
            this.checkBoxMaster.Text = "Master";
            this.checkBoxMaster.UseVisualStyleBackColor = true;
            this.checkBoxMaster.CheckedChanged += new System.EventHandler(this.checkBoxMaster_CheckedChanged);
            // 
            // OEMPAFormExample
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(710, 572);
            this.Controls.Add(this.checkBoxMaster);
            this.Controls.Add(this.buttonNewDevice);
            this.Controls.Add(this.textBoxWriteRange);
            this.Controls.Add(this.textBoxWriteStart);
            this.Controls.Add(this.groupBoxWizard);
            this.Controls.Add(this.buttonWriteStep);
            this.Controls.Add(this.buttonWriteResolution);
            this.Controls.Add(this.buttonResetEncoder);
            this.Controls.Add(this.labelStep);
            this.Controls.Add(this.textBoxStep);
            this.Controls.Add(this.labelResolution);
            this.Controls.Add(this.textBoxResolution);
            this.Controls.Add(this.comboBoxSynchronisation);
            this.Controls.Add(this.buttonLoad);
            this.Controls.Add(this.textBoxWriteGain);
            this.Controls.Add(this.buttonWrite);
            this.Controls.Add(this.textBoxRead);
            this.Controls.Add(this.buttonRead);
            this.Controls.Add(this.textBoxStatus);
            this.Controls.Add(this.textBoxIPAddress);
            this.Controls.Add(this.groupBoxSynchronisation);
            this.Controls.Add(this.groupBoxMediumLevelAPI);
            this.Controls.Add(this.groupBoxLowLevelAPI);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "OEMPAFormExample";
            this.Text = "OEMPAFormExample";
            this.groupBoxLowLevelAPI.ResumeLayout(false);
            this.groupBoxLowLevelAPI.PerformLayout();
            this.groupBoxUSB3.ResumeLayout(false);
            this.groupBoxUSB3.PerformLayout();
            this.groupBoxMediumLevelAPI.ResumeLayout(false);
            this.groupBoxMediumLevelAPI.PerformLayout();
            this.groupBoxWizard.ResumeLayout(false);
            this.groupBoxWizard.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBoxConnect;
        private System.Windows.Forms.TextBox textBoxIPAddress;
        private System.Windows.Forms.TextBox textBoxStatus;
        private System.Windows.Forms.Button buttonRead;
        private System.Windows.Forms.TextBox textBoxRead;
        private System.Windows.Forms.Button buttonWrite;
        private System.Windows.Forms.TextBox textBoxWriteGain;
        private System.Windows.Forms.CheckBox checkBoxPulser;
        private System.Windows.Forms.Button buttonLoad;
        private System.Windows.Forms.TextBox textBoxFileName;
        private System.Windows.Forms.TextBox textBoxFileStatus;
        private System.Windows.Forms.ComboBox comboBoxSynchronisation;
        private System.Windows.Forms.TextBox textBoxResolution;
        private System.Windows.Forms.Label labelResolution;
        private System.Windows.Forms.Label labelStep;
        private System.Windows.Forms.TextBox textBoxStep;
        private System.Windows.Forms.Button buttonResetEncoder;
        private System.Windows.Forms.Button buttonWriteResolution;
        private System.Windows.Forms.Button buttonWriteStep;
        private System.Windows.Forms.GroupBox groupBoxSynchronisation;
        private System.Windows.Forms.GroupBox groupBoxLowLevelAPI;
        private System.Windows.Forms.GroupBox groupBoxMediumLevelAPI;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.Button buttonTest;
        private System.Windows.Forms.Label labelIP;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.GroupBox groupBoxWizard;
        private System.Windows.Forms.Button buttonWizardToFile;
        private System.Windows.Forms.Button buttonTemplate;
        private System.Windows.Forms.Button buttonWizardToHw;
        private System.Windows.Forms.Label labelDataLost;
        private System.Windows.Forms.Button buttonStatus;
        private System.Windows.Forms.Button buttonMsgBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxWriteStart;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxWriteRange;
        private System.Windows.Forms.CheckBox checkBoxWizardLinear;
        private System.Windows.Forms.Button buttonNewDevice;
        private System.Windows.Forms.CheckBox checkBoxMaster;
        private System.Windows.Forms.GroupBox groupBoxUSB3;
        private System.Windows.Forms.RadioButton radioButtonEthernet;
        private System.Windows.Forms.RadioButton radioButtonUSB3;
    }
}

