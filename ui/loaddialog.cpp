#include "loaddialog.h"

LoadDialog::LoadDialog(QWidget *parent, Engine * model, EntityType t)
: QDialog(parent), type(t), engine(model)
{

    setAttribute(Qt::WA_DeleteOnClose);

    setWindowModality(Qt::ApplicationModal);

    QGridLayout *mainLayout = new QGridLayout;
    setLayout(mainLayout);

    if(type == FROM){
        createModelDialog("Load From Model", mainLayout);
    }else if(type == TO){
        createModelDialog("Load To Model", mainLayout);
    }else{
        createAnimationDialog(mainLayout);
    }

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    doneButton = new QPushButton("Done", this);
    connect(doneButton, SIGNAL(clicked()), this, SLOT(onDone()));

    //done/cancel
    mainLayout->addWidget(cancelButton, 3, 0);
    mainLayout->addWidget(doneButton, 3, 1);

    //resize(700, 300);
    setFixedHeight(400);
    setFixedWidth(400);
}

void LoadDialog::createModelDialog(QString title, QGridLayout * mainLayout){

    QLabel * MeshLabel = new QLabel("Mesh:", this);
    QLabel * TexLabel = new QLabel("Texture:", this);

    MeshLine = new QLineEdit(this);
    MeshLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    TexLine = new QLineEdit(this);
    TexLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    QPushButton * MeshFind = new QPushButton("Find", this);
    connect(MeshFind, SIGNAL(clicked()), this, SLOT(onOpenMesh()));

    QPushButton * TexFind = new QPushButton("Find", this);
    connect(TexFind, SIGNAL(clicked()), this, SLOT(onOpenTexture()));

    //Mesh
    mainLayout->addWidget(MeshLabel, 0, 0);
    mainLayout->addWidget(MeshLine, 0, 1, 1, 1);
    mainLayout->addWidget(MeshFind, 0, 2);

    //Texture
    mainLayout->addWidget(TexLabel, 1, 0);
    mainLayout->addWidget(TexLine, 1, 1, 1, 1);
    mainLayout->addWidget(TexFind, 1, 2);

    setWindowTitle(title);
}

void LoadDialog::createAnimationDialog(QGridLayout * mainLayout){
    QString title = "Load Animation";

    AnimLine = new QLineEdit(this);
    QLabel * AnimLabel = new QLabel("Anim:", this);
    AnimLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QPushButton * AnimFind = new QPushButton("Find", this);
    connect(AnimFind, SIGNAL(clicked()), this, SLOT(onOpenAnim()));

    mainLayout->addWidget(AnimLabel, 0, 0);
    mainLayout->addWidget(AnimLine, 0, 1, 1, 1);
    mainLayout->addWidget(AnimFind, 0, 2);

    setWindowTitle(title);
}

QSize LoadDialog::minimumSizeHint() const{
    return QSize(400, 400);
}

QSize LoadDialog::sizeHint() const{
    return QSize(400, 400);
}

void LoadDialog::onDone(){

    ProcedureResult res(false, "");

    if(type == ANIM){
        QString anim = AnimLine->text();

        if(anim.isEmpty()){
            res.setMessage("You have not provided a path for a .md5anim file");
        }
        else{
            res = engine->LoadAnimation(anim); //should be a global string
        }

        if(res.getResult()){
            accept();
        }else{
            QMessageBox::warning(this, tr("Problem loading the Animation: "), res.getMessage(), QMessageBox::Ok);
        }

    }else{
        QString mesh = MeshLine->text();
        QString tex = TexLine->text();

        if(mesh.isEmpty()){
            res.setMessage("You have not provided a path for a .md5model file");
        }
        else if(tex.isEmpty()){
            res = engine->LoadModel(mesh, QDir::currentPath() + NO_TEXTURE_PATH, type);
        }
        else{
            res = engine->LoadModel(mesh, tex, type);
        }

        if(res.getResult()){
            accept();
        }else{
            QMessageBox::warning(this, tr("Problem loading the model: "), res.getMessage(), QMessageBox::Ok);
        }
    }
}

void LoadDialog::onOpenTexture(){
        QString filename = QFileDialog::getOpenFileName(this,tr("Open Texture"), QDir::currentPath(), tr("TGA files (*.tga)"));

        if(!filename.isEmpty()){
            TexLine->clear();
            TexLine->setText(filename);
        }
}

void LoadDialog::onOpenMesh(){
        QString filename = QFileDialog::getOpenFileName(this,tr("Open Model"), QDir::currentPath(), tr("MD5 mesh files (*.md5mesh)"));

        if(!filename.isEmpty()){
            MeshLine->clear();
            MeshLine->setText(filename);
        }
}

void LoadDialog::onOpenAnim(){
        QString filename = QFileDialog::getOpenFileName(this,tr("Open Animation"), QDir::currentPath(), tr("MD5 anim files (*.md5anim)"));

        if(!filename.isEmpty()){
            AnimLine->clear();
            AnimLine->setText(filename);
        }
}
