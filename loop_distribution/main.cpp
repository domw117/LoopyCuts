/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <wrap/qt/anttweakbarMapper.h>
#include "glwidget.h"
#include <QWindow>
#include <QFileInfo>

extern CMesh mesh;
extern std::string pathM;
extern std::string pathF;
extern std::string pathS;
extern bool has_features;
extern bool batch_process;
extern bool delete_unref;
extern bool add_sing_nodes;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWindow dummy;
    QString def_string = QString("GLOBAL fontscaling=%1").arg((int)dummy.devicePixelRatio());
    TwDefine(def_string.toStdString().c_str());
    printf("%s\n",qPrintable(def_string));
    fflush(stdout);

    // Set functions to handle string copy
    TwCopyCDStringToClientFunc(CopyCDStringToClient);
    TwCopyStdStringToClientFunc(CopyStdStringToClient);

    if( !TwInit(TW_OPENGL, NULL) )
    {
        fprintf(stderr, "AntTweakBar initialization failed: %s\n", TwGetLastError());
        return 1;
    }

    //PARAMETERS CHECK
    if(argc<2)
    {
        printf("error: pass mesh name as parameter \n");
        fflush(stdout);
        exit(0);
    }

    //MESH LOAD
    pathM=std::string(argv[1]);
    QString pathMQ=QString(pathM.c_str());
    QFileInfo f_infoM(pathMQ);
    if (!f_infoM.exists())
    {
        std::cout<<"error: mesh fileneme wrong"<<std::endl;
        fflush(stdout);
        exit(0);
    }
    else
        std::cout<<"Mesh file correct"<<std::endl;


    //FIELD LOAD
    pathF=pathM;
    pathF.erase(pathF.find_last_of("."));
    pathF.append(".rosy");

    QString pathFQ=QString(pathF.c_str());
    QFileInfo f_infoF(pathFQ);
    if (!f_infoF.exists())
    {
        printf("error: field fileneme wrong\n");
        fflush(stdout);
        exit(0);
    }
    else
        std::cout<<"Field file correct"<<std::endl;

    pathS=pathM;
    pathS.erase(pathS.find_last_of("."));
    pathS.append(".sharp");
    QString pathSQ=QString(pathS.c_str());
    QFileInfo f_infoS(pathSQ);
    if (!f_infoS.exists())
    {
        printf("no feature line \n");
        has_features=false;
        fflush(stdout);
        //exit(0);
    }
    else
    {
       has_features=true;
       std::cout<<"Sharp file correct"<<std::endl;
    }


    batch_process=false;
    if (argc>=3)
    {
        //then check if it mush batch process
        std::string pathComm;
        pathComm=std::string(argv[2]);
        if (pathComm==std::string("batch"))batch_process=true;
        if (batch_process)
        {
            std::cout<<"*** BATCH PROCESSING ***"<<std::endl;
            std::cout<<"* DATASET "<<pathM.c_str()<<" *"<<std::endl;
        }
    }
    if (argc>=4)
    {
       delete_unref=(bool)atoi(argv[3]);
       if (delete_unref)
          std::cout<<"must delete unref nodes"<<std::endl;
       else
          std::cout<<"keep unref nodes"<<std::endl;
    }
    if (argc>=5)
    {
       add_sing_nodes=(bool)atoi(argv[4]);
       if (add_sing_nodes)
          std::cout<<"must add sing nodes"<<std::endl;
       else
          std::cout<<"no sing nodes"<<std::endl;
    }
    GLWidget window;
    window.show();
    return app.exec();
}
