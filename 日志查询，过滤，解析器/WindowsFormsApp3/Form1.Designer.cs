namespace WindowsFormsApp3
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.B_获取ID_名字对应表 = new System.Windows.Forms.Button();
            this.B_填充名字ID记录到LOG = new System.Windows.Forms.Button();
            this.B_过滤日志 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // B_获取ID_名字对应表
            // 
            this.B_获取ID_名字对应表.Location = new System.Drawing.Point(343, 208);
            this.B_获取ID_名字对应表.Name = "B_获取ID_名字对应表";
            this.B_获取ID_名字对应表.Size = new System.Drawing.Size(225, 23);
            this.B_获取ID_名字对应表.TabIndex = 0;
            this.B_获取ID_名字对应表.Text = "获取ID_名字对应表";
            this.B_获取ID_名字对应表.UseVisualStyleBackColor = true;
            this.B_获取ID_名字对应表.Click += new System.EventHandler(this.button1_Click);
            // 
            // B_填充名字ID记录到LOG
            // 
            this.B_填充名字ID记录到LOG.Location = new System.Drawing.Point(343, 237);
            this.B_填充名字ID记录到LOG.Name = "B_填充名字ID记录到LOG";
            this.B_填充名字ID记录到LOG.Size = new System.Drawing.Size(225, 23);
            this.B_填充名字ID记录到LOG.TabIndex = 1;
            this.B_填充名字ID记录到LOG.Text = "填充名字ID记录到LOG";
            this.B_填充名字ID记录到LOG.UseVisualStyleBackColor = true;
            this.B_填充名字ID记录到LOG.Click += new System.EventHandler(this.B_填充名字ID记录到LOG_Click);
            // 
            // B_过滤日志
            // 
            this.B_过滤日志.Location = new System.Drawing.Point(343, 181);
            this.B_过滤日志.Name = "B_过滤日志";
            this.B_过滤日志.Size = new System.Drawing.Size(225, 23);
            this.B_过滤日志.TabIndex = 2;
            this.B_过滤日志.Text = "过滤日志";
            this.B_过滤日志.UseVisualStyleBackColor = true;
            this.B_过滤日志.Click += new System.EventHandler(this.B_过滤日志_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(911, 384);
            this.Controls.Add(this.B_过滤日志);
            this.Controls.Add(this.B_填充名字ID记录到LOG);
            this.Controls.Add(this.B_获取ID_名字对应表);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button B_获取ID_名字对应表;
        private System.Windows.Forms.Button B_填充名字ID记录到LOG;
        private System.Windows.Forms.Button B_过滤日志;
    }
}

