using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using csDriverOEMPA;

namespace OEMPAFormExample
{
    public partial class FormMsgBox : Form
    {

        public csHWDeviceOEMPA hwDeviceOEMPA;

        public FormMsgBox(csHWDeviceOEMPA _hwDeviceOEMPA)
        {
            hwDeviceOEMPA = _hwDeviceOEMPA;
            InitializeComponent();
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            listBoxMsg.Items.Clear();
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void checkBoxMsgHookEnable_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxMsgHookEnable.Checked)
            {
                csDriverOEMPA.csMsgBox.SetCallbackSystemMessageBox(SystemMessageBox);
                csDriverOEMPA.csMsgBox.SetCallbackSystemMessageBoxList(SystemMessageBoxList);
                csDriverOEMPA.csMsgBox.SetCallbackSystemMessageBoxButtons(SystemMessageBoxButtons);
                csDriverOEMPA.csMsgBox.SetCallbackOempaApiMessageBox(SystemMessageBoxButtons);
            }
            else
            {
                csDriverOEMPA.csMsgBox.SetCallbackSystemMessageBox(null);
                csDriverOEMPA.csMsgBox.SetCallbackSystemMessageBoxList(null);
                csDriverOEMPA.csMsgBox.SetCallbackSystemMessageBoxButtons(null);
                csDriverOEMPA.csMsgBox.SetCallbackOempaApiMessageBox(null);
            }
        }
        public void SystemMessageBox(String pMsg)
        {
            MessageBox.Show(this, pMsg, "OEMPAFormExample", MessageBoxButtons.OK);
        }
        public void SystemMessageBoxList(String pInfo)
        {
            ListAddMessage(pInfo);
        }
        public csEnumMsgBoxReturn SystemMessageBoxButtons(String pMsg, String pTitle, csEnumMsgBoxButtons nType)
        {
            MessageBoxButtons button = MessageBoxButtons.OK;
            DialogResult ret;

            switch(nType)
            {
                case csEnumMsgBoxButtons.csOK:              button = MessageBoxButtons.OK;break;
                case csEnumMsgBoxButtons.csOKCANCEL:        button = MessageBoxButtons.OKCancel;break;
                case csEnumMsgBoxButtons.csABORTRETRYIGNORE:button = MessageBoxButtons.AbortRetryIgnore;break;
                case csEnumMsgBoxButtons.csYESNOCANCEL:     button = MessageBoxButtons.YesNoCancel;break;
                case csEnumMsgBoxButtons.csYESNO:           button = MessageBoxButtons.YesNo;break;
                case csEnumMsgBoxButtons.csRETRYCANCEL:     button = MessageBoxButtons.RetryCancel;break;
            }
            ret = ShowMessageBox(pMsg, pTitle, button);
            switch (ret)
            {
                case DialogResult.None:   return csEnumMsgBoxReturn.csABORT;
                case DialogResult.OK:     return csEnumMsgBoxReturn.csOK;
                case DialogResult.Cancel: return csEnumMsgBoxReturn.csCANCEL;
                case DialogResult.Abort:  return csEnumMsgBoxReturn.csABORT;
                case DialogResult.Retry:  return csEnumMsgBoxReturn.csRETRY;
                case DialogResult.Ignore: return csEnumMsgBoxReturn.csIGNORE;
                case DialogResult.Yes:    return csEnumMsgBoxReturn.csYES;
                case DialogResult.No:     return csEnumMsgBoxReturn.csNO;
            }
            return csEnumMsgBoxReturn.csABORT;
        }

        //http://msdn.microsoft.com/en-us/library/ms171728(v=vs.85).aspx
        delegate void ListAddMessageDelegate(String Msg);
        void ListAddMessage(String Msg)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (this.listBoxMsg.InvokeRequired)
                this.Invoke(new ListAddMessageDelegate(ListAddMessage), new object[] { Msg });
            else
            {
                listBoxMsg.Items.Add(Msg);
                int visibleItems = listBoxMsg.ClientSize.Height / listBoxMsg.ItemHeight;
                listBoxMsg.TopIndex = Math.Max(listBoxMsg.Items.Count - visibleItems + 1, 0);
            }
        }
        delegate DialogResult ShowMessageBoxDelegate(String Msg, String Title, MessageBoxButtons nType);
        DialogResult ShowMessageBox(String Msg, String Title, MessageBoxButtons nType)
        {
            if (this.InvokeRequired)
                return (DialogResult)this.Invoke(new ShowMessageBoxDelegate(ShowMessageBox), new object[] { Msg, Title, nType });
            else
            {
                return MessageBox.Show(this, Msg, "Title", nType);
            }
        }

    }
}
