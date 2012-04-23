#include <QtGui>
#include <QTimer>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "displist.h"
#include "app_fcts.h"

Displist::Displist(QWidget *parent) 
  : QDialog(parent)
{

  QLabel *label_try;
  QLabel *autoMsm;
  QLabel *manualMsm;
  QLabel *sep;
  Bpause=0;

  if ((sockfd = make_socket()) < 0)
    init_flag = 1;
  else
      init_flag = 2;
  no_lines = 0;
  crt_line = 0;
  autosend_status=0;


  timerCount = new QTimer(this);
  connect(timerCount, SIGNAL(timeout()), this, SLOT(runningTime()));
  timerCount->setInterval(10); //every second

  sendShot = new QTimer(this);
  connect(sendShot, SIGNAL(timeout()), this, SLOT(autoSendtext()));

  list_wid = new QListWidget(this);
  list_wid->addItem("\n");
  list_wid->addItem("\nLoad new File\n");
  list_wid->addItem("\n");
  list_wid->setCurrentRow(1);

  item_font = list_wid->font();
  item_font.setPixelSize(25);
  item_font.setFamily("Times");

  list_wid->setSpacing(30);
  list_wid->setFont(item_font);
  
  load_file = new QAction(tr("&Load file"), this);
  //load_file->setIcon(QIcon":/img/load.png");
  load_file->setShortcut(tr("Ctrl+O"));
  load_file->setStatusTip(tr("Load subtitle file"));
  connect(load_file, SIGNAL(triggered()), this, SLOT(load_dialog()));

  exit_app = new QAction("&Exit", this);
  //exit_app->setIcon(QIcon(":/img/exit.png"));
  exit_app->setShortcut(tr("Ctrl+Q"));
  exit_app->setStatusTip(tr("Exit the application"));
  connect(exit_app, SIGNAL(triggered()), this, SLOT(close()));

  help_about = new QAction(tr("&About"), this);
  //help_about->setIcon(QIcon(":/img/about.png"));
  help_about->setShortcut(tr("Ctrl+H"));
  help_about->setStatusTip(tr("About the application"));
  connect(help_about, SIGNAL(triggered()), this, SLOT(about_dialog()));

  QFont font;
  font.setPointSize(20);
  font.setBold(true);
  autoMsm = new QLabel("Automatic controls:");
  manualMsm = new QLabel("Manual controls:");
  label_try = new QLabel("Use up/down arrows \nto move through the subtitles,\nEnter to send");
  lTimer = new QLabel(" 00:00:00,00");
  lTimer->setFont(font); 
  totalLines = new QLabel("Lines: 0/0");
  sep = new QLabel(this);
  sep->setFrameStyle(QFrame::HLine | QFrame::Plain);
  stat_bar = new QStatusBar();
  menu_bar = new QMenuBar();
  file_menu = new QMenu("&File");
  help_menu = new QMenu("&Help");

  QPushButton *sButton = new QPushButton(tr("Start"));
  QObject::connect(sButton, SIGNAL(clicked()),this, SLOT(startauto()));

  QPushButton *pButton = new QPushButton(tr("Pause"));
  QObject::connect(pButton, SIGNAL(clicked()),this, SLOT(pauseauto()));

  QPushButton *qButton = new QPushButton(tr("Stop"));
  QObject::connect(qButton, SIGNAL(clicked()),this, SLOT(stopauto()));


  file_menu->addAction(load_file);
  file_menu->addSeparator();
  file_menu->addAction(exit_app);
  help_menu->addAction(help_about);
  menu_bar->addMenu(file_menu);
  menu_bar->addSeparator();
  menu_bar->addMenu(help_menu);

  QVBoxLayout *mainlayout = new QVBoxLayout(this);
  QHBoxLayout *horizlayout = new QHBoxLayout(this);
  QVBoxLayout *verticallayout = new QVBoxLayout(this);

  QGroupBox* agridGroupBox = new QGroupBox(tr("Automatic controls"));
  QGroupBox* mgridGroupBox = new QGroupBox(tr("Manual controls"));


  horizlayout->addWidget(list_wid);
  
  QGridLayout *aLayout = new QGridLayout;

  horizlayout->addLayout(verticallayout);
  verticallayout->addWidget(agridGroupBox);
  verticallayout->addWidget(mgridGroupBox);

  aLayout->addWidget(sButton,0,0,Qt::AlignCenter);
  aLayout->addWidget(pButton,1,0,Qt::AlignCenter);
  aLayout->addWidget(qButton,2,0,Qt::AlignCenter);
  aLayout->addWidget(lTimer,3,0,Qt::AlignLeft);
  aLayout->addWidget(totalLines,4,0,Qt::AlignLeft);
  agridGroupBox->setLayout(aLayout);


  QGridLayout *mLayout = new QGridLayout;

  mLayout->addWidget(label_try,0,Qt::AlignCenter);
  mgridGroupBox->setLayout(mLayout);

  mainlayout->addWidget(menu_bar);
  mainlayout->addLayout(horizlayout);
  mainlayout->addWidget(stat_bar);
  setLayout(mainlayout); 
}

void Displist::keyPressEvent(QKeyEvent *event)
{
  QListWidgetItem *it;
  string str;
  if (init_flag != 0)
    return;

  if(autosend_status == 0)
  {
    switch (event->key()) {
     case Qt::Key_Enter:
     case Qt::Key_Return:

      /* even if not all bytes are sent for now it's ok!! */
      if (send_msg(CLIENTIP, sockfd, lines[crt_line]) > 0) {

        stat_bar->showMessage(lines[crt_line], 0);
        /* first row gets is the line sent */
        it = list_wid->item(0);
        str="\n";
        str.append(teStamps[crt_line]);
        str.append(" | ");
        str.append(lines[crt_line]);
        str.append("\n");
        it->setData(Qt::DisplayRole, str.c_str());

        /* increment array index */
        crt_line++;
      
        /* check array bounds */
        if (crt_line > (no_lines - 1))
	  crt_line = no_lines - 1;
      
        /* current row is the line below the line sent !? */
        //      it = list_wid->currentItem();
        it = list_wid->item(1);
        if (crt_line == no_lines)
	  it->setData(Qt::DisplayRole, "\n");
        else
        {
          str="\n";
          str.append(teStamps[crt_line]);
          str.append(" | ");
          str.append(lines[crt_line]);
          str.append("\n");
          it->setData(Qt::DisplayRole, str.c_str());
	}
      
        /* last row... depends */ 
        it = list_wid->item(2);
        if (crt_line == (no_lines - 1))
	  it->setData(Qt::DisplayRole, "\n");
        else
	{
          str="\n";
          str.append(teStamps[crt_line+1]);
          str.append(" | ");
          str.append(lines[crt_line+1]);
          str.append("\n");
          it->setData(Qt::DisplayRole, str.c_str());
	}
      }
      update();
      break;
     case Qt::Key_Down:

       /* increment array index */
       crt_line++;
      
       /* check array bounds */
       if (crt_line > (no_lines - 1))
	crt_line = no_lines - 1;

       /* last row... depends */ 
       it = list_wid->item(2);
       if (crt_line == (no_lines - 1))
	it->setData(Qt::DisplayRole, "\n");
       else
       {
	str="\n";
        str.append(teStamps[crt_line+1]);
        str.append(" | ");
        str.append(lines[crt_line+1]);
        str.append("\n");
        it->setData(Qt::DisplayRole, str.c_str());
       }
      
       /* current row gets back 2nd row*/
       //      list_wid->setCurrentRow(list_wid->currentRow() - 1);
       list_wid->setCurrentRow(1);
       it = list_wid->item(1);
       if (crt_line == no_lines)
	it->setData(Qt::DisplayRole, "\n");
       else
       {
         str="\n";
         str.append(teStamps[crt_line]);
         str.append(" | ");
         str.append(lines[crt_line]);
         str.append("\n");
         it->setData(Qt::DisplayRole, str.c_str());
        }

       /* first row */
       it = list_wid->item(0);
       str="\n";
       str.append(teStamps[crt_line-1]);
       str.append(" | ");
       str.append(lines[crt_line-1]);
       str.append("\n");
       it->setData(Qt::DisplayRole, str.c_str());
      
       update();
      break;
     case Qt::Key_Up:

       /* decrement array index */
       crt_line--;
      
       /* check array bounds */
       if (crt_line < 0)
	crt_line = 0;

       list_wid->clearSelection();

       /* first row... depends */ 
       it = list_wid->item(0);
       if (crt_line == 0)
	 it->setData(Qt::DisplayRole, "\n");
       else
       {
	str="\n";
        str.append(teStamps[crt_line-1]);
        str.append(" | ");
        str.append(lines[crt_line-1]);
        str.append("\n");
        it->setData(Qt::DisplayRole, str.c_str());
       }

       /* current row gets back 2nd row*/
       //list_wid->setCurrentRow(list_wid->currentRow() + 1);
       list_wid->setCurrentRow(1);
       it = list_wid->item(1);
       str="\n";
       str.append(teStamps[crt_line]);
       str.append(" | ");
       str.append(lines[crt_line]);
       str.append("\n");
       it->setData(Qt::DisplayRole, str.c_str());

       /* last row */
       it = list_wid->item(2);
       str="\n";
       str.append(teStamps[crt_line+1]);
       str.append(" | ");
       str.append(lines[crt_line+1]);
       str.append("\n");
       it->setData(Qt::DisplayRole, str.c_str());
       update();
      break;
    default:
    QDialog::keyPressEvent(event);
   }
  }
}

int Displist::getFailInit()
{
    return init_flag;
}

void Displist::load_dialog()
{
  QListWidgetItem *it;
  QString file;
  string str;
  char buff[20];

  file = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text Files (*.txt);; Subtitle files (*.srt)"));

  strcpy(filename, (file.toAscii()).data());
 
  if(strlen(filename)== 0)
      return;

  printf("file name: %s\n", filename);
  
  if (no_lines > 0)
    free_array(lines, no_lines);

  crt_line = 0;

  //if ((no_lines = read_file(filename, lines)) <= 0) { //old read files
  if ((no_lines = load_sub_file()) <= 0) {
    init_flag = 2;
    no_lines = 0;

    it = list_wid->item(0);
    it->setData(Qt::DisplayRole, "\n");
    it = list_wid->item(1);
    it->setData(Qt::DisplayRole, "\n");
    list_wid->setCurrentRow(1);
    it = list_wid->item(2);
    it->setData(Qt::DisplayRole, "\n");
  } else {
    init_flag = 0;
    loadTstamp();
    it = list_wid->item(0);
    it->setData(Qt::DisplayRole, "\n");
    it = list_wid->item(1);
    str.append("\n");
    str.append(teStamps[crt_line]);
    str.append(" | ");
    str.append(lines[crt_line]);
    str.append("\n");
    it->setData(Qt::DisplayRole, str.c_str());
    list_wid->setCurrentRow(1);
    it = list_wid->item(2);
    str="\n";
    str.append(teStamps[crt_line+1]);
    str.append(" | ");
    str.append(lines[crt_line+1]);
    str.append("\n");
    it->setData(Qt::DisplayRole, str.c_str());
    sprintf(buff,"Lines: %d/%d",crt_line,no_lines);
    totalLines->setText(buff);
    update();
  }
}

int Displist::load_sub_file() 
{

   ifstream indata; // indata is like cin
   size_t pos_i;
   size_t pos_t;
   char str[MAX_LINE_CHARS];
   int i,len;
   string line;
   string cline;
   string auxline="";
   char endcline=1; // 1 end found -- 0 end not found
   char nlines=0;
  
   indata.open(filename); // opens the file
   if(!indata) { // file couldn't be opened
      QMessageBox::critical(this, tr("No File"),tr("<h4>File format error</h4>"),QMessageBox::Ok);
      return -1;
   }

   indata.getline(str, MAX_LINE_CHARS, '\n'); 
   while ( !indata.eof() ) // keep reading until end-of-file
   {
    line=str;
    pos_i = line.find("<i>");
    pos_t = line.find("</t>");
    if(pos_t !=string::npos && endcline && pos_i !=string::npos)
    {
          auxline=line.substr(pos_i,pos_t+4);
          len=strlen(auxline.c_str());
          if ((lines[nlines] = (char*) calloc(len, sizeof(char))) == NULL) {
                cout<<"not enough heap memory"<<endl;
                return(-6);
           }
          
          memcpy(lines[nlines++],auxline.c_str(),len);
//          cout << auxline << endl;
	  auxline="";
    }
    else
    {
        if(pos_t == string::npos)
	 {
	    if(pos_i != string::npos)
            {
                 auxline.append(line.substr(pos_i,line.length()-1));
            }
            else	
            {
	         auxline.append(line);
            }
            endcline = 0;
            //cout<<auxline<<endl;
         }
	 else
         {
 	     auxline.append(line.substr(0,pos_t+5));
             len=strlen(auxline.c_str());
             if ((lines[nlines] = (char*) calloc(len, sizeof(char))) == NULL) {
                printf("not enough heap memory");
                return(-6);
             }
            memcpy(lines[nlines++],auxline.c_str(),len);
 //           cout << lines[nlines-1] << endl;
            endcline = 1;
            //cout<<"endcline = 1"<<endl;
  	    auxline="";
         }
    }
  
    indata.getline(str, MAX_LINE_CHARS, '\n'); 
   }

   indata.close();
//   cout << "End-of-file reached.." << endl;
   return nlines;
}

void Displist::about_dialog()
{
  QMessageBox::about(this, tr("About Subtitler"), tr("<h2>Subtitler 0.0</h2>" 
						     "<p>Copyright &copy; UAB 2010"
						     "<p>The subtitler is an application"
						     "used for sending subtitles to "
						     "spectators in theaters."));
}

void Displist::startauto()
{
   string str="";
   QListWidgetItem *it;
   int pausetime=0;

   if(Bpause)
   {
     pausetime=(hour*3600+minute*60+second)*1000+mili;
     cout<< pausetime << " ";

     pausetime=tStamps[crt_line]-pausetime;

     //cout<< tStamps[crt_line] << "=" << pausetime << "\n" << endl;
     sendShot->setInterval(pausetime); //every second
     sendShot->start();
     timerCount->start();
     Bpause=0;
     return;
   }

   if(no_lines== 0)
   {
      QMessageBox::warning(this, tr("No XML File"),tr("<h4>Load XML file</h4>"),QMessageBox::Ok);
     return;
   }
   crt_line=0;
    
   sendShot->setInterval(tStamps[crt_line]); //every second
   hour=0;
   minute=0;
   second=0;
   mili=0;
   it = list_wid->item(0);
   it->setData(Qt::DisplayRole, "\n");
   it = list_wid->item(1);
   str.append("\n");
   str.append(teStamps[crt_line]);
   str.append(" | ");
   str.append(lines[crt_line]);
   str.append("\n");
   it->setData(Qt::DisplayRole, str.c_str());
   list_wid->setCurrentRow(1);
   it = list_wid->item(2);
   str="\n";
   str.append(teStamps[crt_line+1]);
   str.append(" | ");
   str.append("\n");
   str.append(lines[crt_line+1]);
   it->setData(Qt::DisplayRole, str.c_str());
   update();
   timerCount->start();
   sendShot->start();
   lTimer->setText(" 00:00:00,00");
   autosend_status=1;
//   update();
}

void Displist::pauseauto()
{
   timerCount->stop();
   sendShot->stop();
   Bpause=1;
}

void Displist::stopauto()
{
   autosend_status=0;
   timerCount->stop();
   sendShot->stop();
 //  cout << "Start: 0" << hour << ":0" << minute << ":0" << second << endl;
}
void Displist::runningTime()
{
   char sTime[17];
   char cH[4];
   char cM[4];
   char cS[4];

   mili+=10;
   if(mili>=1000)
   {
    second++;
    mili=0;
    if(second == 60)
    {
      second=0;
      minute++;
      if(minute == 60)
      {
         minute=0;
         hour++;
      }
    }
   }
 
   

   if(hour<10)   
     sprintf(cH,"0%d",hour);     
   else
     sprintf(cH,"%d",hour);     

   if(minute<10)   
     sprintf(cM,"0%d",minute);     
   else
     sprintf(cM,"%d",minute);     

   if(second<10)   
     sprintf(cS,"0%d",second);     
   else
     sprintf(cS,"%d",second);     
   
   if(mili!=0)
   { 
     if(mili>=100)
     {
       sprintf(sTime," %s:%s:%s,%d",cH,cM,cS,mili/10);
     }
     else
     {
       sprintf(sTime," %s:%s:%s,%d",cH,cM,cS,mili/10);
     }
   }
   else
     sprintf(sTime," %s:%s:%s,00",cH,cM,cS);

   lTimer->setText(sTime);
//   update();
}
void Displist::autoSendtext()
{
   QListWidgetItem *it;
   int nextTime;
   string str;
   char buff[25];

   if (send_msg(CLIENTIP, sockfd, lines[crt_line]) > 0) 
   {

      stat_bar->showMessage(lines[crt_line], 0);
       /* first row gets is the line sent */
      it = list_wid->item(0);
      str="\n";
      str.append(teStamps[crt_line]);
      str.append(" | ");
      str.append(lines[crt_line]);
      str.append("\n");
      it->setData(Qt::DisplayRole,str.c_str());
      /* increment array index */
      crt_line++;
      
      /* current row is the line below the line sent !? */
      //      it = list_wid->currentItem();

      list_wid->setCurrentRow(1);
      it = list_wid->item(1);
      if (crt_line == no_lines)
	it->setData(Qt::DisplayRole, "\n");
      else
      {
        str="\n";
        str.append(teStamps[crt_line]);
        str.append(" | ");
        str.append(lines[crt_line]);
        str.append("\n");
        it->setData(Qt::DisplayRole, str.c_str());
      }
      
      /* last row... depends */ 
      it = list_wid->item(2);
      if (crt_line >= (no_lines - 1))
	it->setData(Qt::DisplayRole, "\n");
      else
      {
	str="\n";
        str.append(teStamps[crt_line+1]);
        str.append(" | ");
        str.append(lines[crt_line+1]);
        str.append("\n");
	it->setData(Qt::DisplayRole,str.c_str());
      }
    }
    update();
  
    if(autosend_status && crt_line < no_lines)
    {
      nextTime=tStamps[crt_line]-tStamps[crt_line-1];
      sendShot->stop();
      sendShot->setInterval(nextTime);
      sendShot->start();
      sprintf(buff,"Lines: %d/%d",crt_line,no_lines);
      //printf("Lines: %d/%d\n",crt_line,no_lines);
      totalLines->setText(buff);
    }
    else
    {
      stopauto();
      //printf("STOP\n");
    }
}
void Displist::loadTstamp()
{ 
   int i;
   string subText;
   string teSt;
 
   for(i=0;i<no_lines;i++)
   {
      scanLinetags(lines[i],&subText,&teSt,&tStamps[i]);
      strcpy(lines[i],subText.c_str());
      strcpy(teStamps[i],teSt.c_str());
   }
}
void Displist::scanLinetags(string line, string* subText,string* teSt,int* timeStamp)
{
  string str;
  size_t pos,pos2,last_p;
  int hours;
  int minutes;
  int seconds;
  int milis;

  //cout << "mm: "<< line << endl;
  pos= line.find("<i>");
  pos2= line.find("</i>");
  str=line.substr(pos+3,(pos2-pos-3));
  //printf("blanc: %d\n",strlen(str.c_str()));
  if(strlen(str.c_str()) == 0)
  {
    *subText = " ";
  }
  else
  {
   *subText= str;
  }
  //cout << "mm: "<< subText << endl;
  pos= line.find("<t>");
  pos2= line.find("</t>");
  str= line.substr(pos+3,(pos2-pos-3));
  *teSt=str;
  pos = str.find(":");
  last_p = str.find_last_of(":");
  pos2 = str.find(",");
  hours= atoi(str.substr(0,pos).c_str());
  minutes= atoi(str.substr(pos+1,last_p).c_str());
  seconds= atoi(str.substr(last_p+1,pos2).c_str());
  milis= atoi(str.substr(pos2+1).c_str());
  cout << "t: "<<hours<< " -- "<< minutes << " -- "<<seconds<<" -- "<< milis << " -- " <<str << endl;
  *timeStamp = (hours*3600+minutes*60+seconds)*1000; //milliseconds
  //cout << "t: "<< minutes << " -- "<<seconds<<" -- "<< str << endl;
}

Displist::~Displist()
{
  if (no_lines > 0)
    free_array(lines, no_lines);
}
