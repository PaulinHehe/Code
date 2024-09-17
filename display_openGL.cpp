#include "display_openGL.h"

int frameWidth, frameHeight;
int frameRenderWidth, frameRenderHeight;
double frameOriginalWidth;
double frameOriginalHeight;
double addFrameX;
double addFrameY;

double actualUsedWidth, actualUsedHeight;
double xmin, ymin, xmax, ymax;
double diviseur;

double aX, aY, bX, bY;
double refOriginalX, refOriginalY;

double offsetOriginalX, offsetOriginalY;
double offsetX, offsetY;

double zoomMolette;
double ancienZoomMolette;

bool draggingViewMode;
bool isDraggingView;
double cursorPosRef0X, cursorPosRef0Y;

double cursorPosX;
double cursorPosY;

bool draggingNodesMode;
bool isDraggingNode;
double hitbox;

bool showIllegalMode;
bool showAllIntersectionMode;
bool showCells;
bool showCircles;

int userAction;
int selectedEmplacement;
int selectedNode;
int oldEmplacement;
long long score;
std::vector<std::pair<int,int>> moveLog;

bool changeCoord;
bool coordOriginalesReels;
bool freezeModifMode;

vector<double> textBuffer;
string text;
RecuitSimule* recuit;
Shader* shader;

/*---INITIALISATION---*/
void init(){
    frameOriginalWidth = 1500;
    frameOriginalHeight = 1000;
    addFrameX = 1;
    addFrameY = 1;
    zoomMolette = 1;
    ancienZoomMolette = 1;
    cursorPosX = 0;
    ancienZoomMolette = 1;
    draggingViewMode = true;
    isDraggingView = false;
    cursorPosX = 0;
    cursorPosY = frameOriginalHeight;
    draggingNodesMode = false;
    isDraggingNode = false;
    showIllegalMode = true;
    showAllIntersectionMode = false;
    showCells = false;
    userAction = -1;
    selectedEmplacement = -1;
    selectedNode = -1;
    showCircles = false;
    score = LLONG_MAX;
    moveLog.push_back({0,0});
    changeCoord = false;
    freezeModifMode = false;
    text = "";
}
void initVariablesGraphe(Graphe& G){
    init();

    xmin = G.getXYmin().getX();
    xmax = G.getXYmax().getX();
    ymin = G.getXYmin().getY();
    ymax = G.getXYmax().getY();

    actualUsedWidth = xmax - xmin;
    actualUsedHeight = ymax - ymin;

    coordOriginalesReels = G.isCoordReel();
}
//Centrer le graphe en fonction des dimensions de l'écran et du graphe
//Ces paramètres seront enreegistrés comme l'état initial de l'affichage du graphe
void initRepere(){ 
    zoomMolette = 1;

    double ratio = ((double)frameHeight/(double)frameWidth - actualUsedHeight/actualUsedWidth) * actualUsedWidth;
    if(ratio >= 0){
        offsetX = 0;
        offsetY = ratio/2;
    }else{
        ratio = (((double)frameWidth/(double)frameHeight) - (actualUsedWidth/actualUsedHeight)) * actualUsedHeight;
        offsetX = ratio/2;
        offsetY = 0;
    }
    offsetOriginalX = offsetX;
    offsetOriginalY = offsetY;

    refOriginalX = xmax + 2*offsetX - xmin;
    refOriginalY = ymax + 2*offsetY - ymin;

    frameOriginalWidth = frameWidth;
    frameOriginalHeight = frameHeight;

    addFrameX = 1;
    addFrameY = 1;
}


/*---DEFINIR LE REPERE POUR AFFICHER LE GRAPHE---*/
//Calcul des coordonées des points a : BasGauche et b : HautDroite de l'écran passés en paramètres à glOrtho à la frame suivante (nouveau répère pour afficher le graphe)
//En fonction du zoom et du décalage (offset)
void repereOrtho(Graphe &G){ 
    double ref1X = refOriginalX/zoomMolette;
    double ref1Y = refOriginalY/zoomMolette;
    
    double off1X = (offsetX/refOriginalX) * ref1X;
    double off1Y = (offsetY/refOriginalY) * ref1Y;

    aX = xmin - off1X;
    aY = ymin - off1Y;
    bX = aX + ref1X*addFrameX;
    bY = aY + ref1Y*addFrameY;

    glOrtho(aX, bX, aY, bY, 0.0f, 1.0f);

    hitbox = max(ref1X, ref1Y) * 0.0075;
}
//Calcul du décalage à afficher à la frame suivante
//En fonction du nouveau zoom qui est centré sur la coordonée du graphe passée en paramètres (peut être à l'extérieur des extrémités du graphe)
void calculNouveauRepereZoomFocal(double cX, double cY){
    double cX0 = (cX - xmin - offsetX + offsetOriginalX) / ancienZoomMolette;
    double cY0 = (cY - ymin - offsetY + offsetOriginalY) / ancienZoomMolette;

    double oX = (xmin - cX) * zoomMolette + cX - xmin;
    double oY = (ymin - cY) * zoomMolette + cY - ymin;

    double cX1 = (cX0) * zoomMolette + oX;
    double cY1 = (cY0) * zoomMolette + oY;

    double tX = cX - cX1 - xmin;
    double tY = cY - cY1 - ymin;

    offsetX = oX + tX + offsetOriginalX;
    offsetY = oY + tY + offsetOriginalY;
}


/*---RENVOYER UN POSITION EN FONCTION D'UN REPERE DONNE---*/
//3 repères possibles : 
//1) En pixels selon la taille de l'écran : (0,0) en HautGauche de l'écran
//2) En coordonées du graphe en fonction de l'affichage original : (xmin,ymin) l'origine de la boite englobante dans l'affichage original
//3) En coordonées du graphe en fonction de l'affichage actuel : (xmin,ymin) l'origine de la boite englobante dans l'affichage actuel
Position<double> getPositionRepereInitialFromPixels(double clicX, double clicY){
    double x = (clicX / (double)frameWidth) * refOriginalX * addFrameX - offsetOriginalX + xmin;
    double y = ((frameHeight - clicY) / (double)frameHeight) * refOriginalY * addFrameY - offsetOriginalY + ymin;

    return Position<double>(x,y);
}
Position<double> getPositionRepereActuelFromPixels(double clicX, double clicY){
    double x = (clicX / (double)frameWidth) * (bX - aX) - offsetX/zoomMolette + xmin;
    double y = ((frameHeight - clicY) / (double)frameHeight) * (bY - aY) - offsetY/zoomMolette + ymin;

    return Position<double>(x,y);
}
Position<double> getPositionfromRepereActuelToInitial(double posX, double posY){
    double x = (posX - aX) / (bX - aX) * refOriginalX - offsetOriginalX + xmin;
    double y = (posY - aY) / (bY - aY) * refOriginalY - offsetOriginalY + ymin;

    return Position<double>(x,y);
}
Position<double> getPositionOrtho(double posX, double posY){
    return Position<double>(posX, posY);
}


/*---DEPLCAER UN NOEUD EN DRAG AND DROP -> CTRL + DRAG LE NOEUD---*/
//Selection d'un noeud si sa distance avec le curseur < à la zone de selection du noeud (hitbox)
//Si plusieurs zones de selection se chevauchent, on sélectionne le noeud le plus proche du curseur
void cursorDansPerimetreEmplacement(Graphe &G){
    if(selectedNode > -1){
        Position<double> cursor = getPositionRepereActuelFromPixels(cursorPosX, cursorPosY);

        std::vector<Position<double>> tabPos(G.getNbEmplacements());
        for(int i=0; i<G.getNbEmplacements(); i++){
            tabPos[i] = G.getEmplacement(i).getPos();
        }
        selectedEmplacement = getNumPositionFromRayon(cursor, tabPos, hitbox);

        G.placeNoeud(selectedNode, oldEmplacement);
        if(selectedEmplacement != -1){
            //cout << "Emplacement : " << selectedEmplacement << " : " <<  G.getEmplacement(selectedEmplacement).getPos().getX() << " x " << G.getEmplacement(selectedEmplacement).getPos().getY() << "\n";
            G.placeNoeud(selectedNode, selectedEmplacement);
            /*if(score != G.getScoreTotal()){
                score = G.getScoreTotal();
                cout << "Score : " << score << "\n";
            }*/
            saveLastMove();
        }
    }
    selectedNode = -1;
    selectedEmplacement = -1;
    isDraggingNode = false;
}
//Selection d'un emplacement si sa distance avec le curseur < à la zone de selection de l'emplacement (hitbox)
//Si plusieurs zones de selection se chevauchent, on sélectionne l'emplacement le plus proche du curseur
void cursorDansPerimetreNoeud(Graphe &G){
    Position<double> cursor = getPositionRepereActuelFromPixels(cursorPosX, cursorPosY);

    std::vector<Position<double>> tabPos(G.getNbNoeuds());
    for(int i=0; i<G.getNbNoeuds(); i++){
        tabPos[i] = G.getNoeud(i).getPos();
    }
    selectedNode = getNumPositionFromRayon(cursor, tabPos, hitbox);
    
    //On place le noeud selectionné à l'emplacement sélectionné
    //Si l'emplacement n'est pa libre, les deux noeuds échangeront leurs emplacements (oldEmplacement <-> selectedEmplacement)
    if(selectedNode != -1){
        //cout << "Noeud : " << selectedNode << " : " <<  G.getNoeud(selectedNode).getPos().getX() << " x " << G.getNoeud(selectedNode).getPos().getY() << "\n";
        /*if(score != G.getScoreTotal()){
            score = G.getScoreTotal();
            cout << "Score : " << score << "\n";
        }*/
        oldEmplacement = G.getNoeud(selectedNode).getEmplacement();
        G.placeNoeud(selectedNode, -1);
        cursor.setX(max(min(xmax, cursor.getX()), xmin));
        cursor.setY(max(min(ymax, cursor.getY()), ymin));
        G.bougeNoeud(selectedNode, cursor);
        isDraggingNode = true;
    }
}
//Modifie les coordonées du noeud pour suivre le curseur
//Borné par la boite englobante du graphe
void dragNoeud(Graphe &G){
    Position<double> cursor = getPositionRepereActuelFromPixels(cursorPosX, cursorPosY);
    cursor.setX(max(min(xmax, cursor.getX()), xmin));
    cursor.setY(max(min(ymax, cursor.getY()), ymin));
    G.bougeNoeud(selectedNode, cursor);
    /*if(score != G.getScoreTotal()){
        score = G.getScoreTotal();
        cout << "Score : " << score << "\n";
    }*/
}
//Execute l'action de modification du graphe demandée par l'utilisateur
void actionUtilisateurSurLeGraphe(Graphe& G){
    if(changeCoord && !coordOriginalesReels){ 
        G.changeTypeCoord();
        changeCoord = false;
    }
    switch(userAction){
        case 0 : 
            cursorDansPerimetreEmplacement( G);
        break;
        case 1 :        
            cursorDansPerimetreNoeud(G);
        break;
        case 2 : 
            dragNoeud(G);
        break;
        case 3 : 
            rewindSavedMove(G);
        break;
        case 4 : 
            forwardSavedMove(G);
        break;
    }
    userAction = -1;
}
void saveLastMove(){
    if(moveLog[0].first == moveLog.size()-1){
        moveLog.push_back({oldEmplacement, selectedEmplacement});
    }else{
        moveLog[moveLog[0].first+1] = {oldEmplacement, selectedEmplacement};
    }
    moveLog[0].first++;
    moveLog[0].second = moveLog[0].first;
}
void rewindSavedMove(Graphe& G){
    if(moveLog[0].first > 0){
        int n = G.getEmplacement(moveLog[moveLog[0].first].second).getNoeud();
        int e = G.getEmplacement(moveLog[moveLog[0].first].first).getId();
        G.placeNoeud(n, e);
        moveLog[0].first--;
    }
}
void forwardSavedMove(Graphe& G){
    if(moveLog[0].first < moveLog[0].second){
        moveLog[0].first++;
        int n = G.getEmplacement(moveLog[moveLog[0].first].first).getNoeud();
        int e = G.getEmplacement(moveLog[moveLog[0].first].second).getId();
        G.placeNoeud(n, e);
    }   
}


/*---REPONSE AUX EVENEMENTS CLAVIERS SOURIS---*/
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
void window_size_callback(GLFWwindow* window, int width, int height) {
	glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    //rapport entre la taille original de la fenêtre et sa nouvelle taille
    addFrameX = ((double)frameWidth/frameOriginalWidth);
    addFrameY = ((double)frameHeight/frameOriginalHeight);
}
static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {   
    ancienZoomMolette = zoomMolette;
    if ((int)yoffset == -1) { // Dezoom
        if(draggingNodesMode){
            zoomMolette *= 1.2;
        }else{
            zoomMolette *= 1.05;
        }
    }
    else if ((int)yoffset == 1 && ancienZoomMolette > 0.05) { // Zoom
        if(draggingNodesMode){
            zoomMolette /= 1.2;
        }else{
            zoomMolette /= 1.05;
        }
    }

   if(ancienZoomMolette != zoomMolette){ 
        //zoom/dezoom avec comme point focal le curseur
        glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
        Position<double> cursor = getPositionRepereInitialFromPixels(cursorPosX, cursorPosY);
        calculNouveauRepereZoomFocal(cursor.getX(), cursor.getY());
    }
}
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    Position<double> cursor = getPositionRepereInitialFromPixels(cursorPosX, cursorPosY);
   
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(draggingViewMode){
            //commencer à déplacer le graphe dans la fenêtre
            isDraggingView = true;
            cursorPosRef0X = cursor.getX();
            cursorPosRef0Y = cursor.getY();
        }else if(draggingNodesMode){
            //selectionner un noeud à déplacer
            userAction = 1;    
        }
    }else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        if(draggingViewMode){
            //terminer de déplacer le graphe dans la fenêtre
            isDraggingView = false;
        }else if(isDraggingNode){
            //placer le noeud selectionné
            userAction = 0;
        }
    }
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
    Position<double> cursor = getPositionRepereInitialFromPixels(cursorPosX, cursorPosY);
    if(draggingViewMode && isDraggingView){        
        //déplacer le graphe dans la fenêtre en fonction de la position du curseur
        offsetX += (cursor.getX() - cursorPosRef0X);
        offsetY += (cursor.getY() - cursorPosRef0Y);

        cursorPosRef0X = cursor.getX();
        cursorPosRef0Y = cursor.getY();
    }else if(draggingNodesMode && isDraggingNode){
        //déplacer le noeud sélectionné en fonction de la position du curseur
        userAction = 2;    
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {   
    if (action == GLFW_PRESS){       
        switch (key) {         
            case GLFW_KEY_ESCAPE: 
                glfwSetWindowShouldClose(window, GLFW_TRUE); 
            break;
            case GLFW_KEY_SPACE: 
                freezeModifMode = !freezeModifMode;
            break;
            case GLFW_KEY_LEFT_CONTROL: //Début du mode déplacement de noeuds
                draggingViewMode = !draggingViewMode;
                draggingNodesMode = !draggingNodesMode;
                changeCoord = true;
            break;
            case GLFW_KEY_C: //Show Cellules de la grille
			    showCircles = !showCircles;
			break;
            case GLFW_KEY_G: //Show Cellules de la grille
			    showCells = !showCells;
			break;
            case GLFW_KEY_I: //Show seulement les intersections illégales (à grosse pénalité) ou toutes les intersections
                if(showIllegalMode){
                    if(!showAllIntersectionMode){
                        showAllIntersectionMode = true;
                    }else if(showAllIntersectionMode){
                        showIllegalMode = !showIllegalMode;
                        showAllIntersectionMode = false;
                    }
                }else{
                    showIllegalMode = !showIllegalMode;
                }
			break;
            case GLFW_KEY_W: //Z sur clavier AZERTY !! // Reset Zoom et décalage (offset)
                initRepere();
			break;
            case GLFW_KEY_1: //Retour en arrière dans les modifications manuelles
                userAction = 3;
			break;
            case GLFW_KEY_2: //Retour en avant dans les modifications manuelles
                userAction = 4;
			break;
            case GLFW_KEY_KP_0: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(0);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_1: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(1);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_2: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(2);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_3: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(3);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_4: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(4);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_5: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(5);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_6: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(6);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_7: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(7);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_KP_8: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(8);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_INSERT: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    textBuffer.push_back(9);
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_BACKSPACE: //Retour en avant dans les modifications manuelles
                if(freezeModifMode && !textBuffer.empty()){
                    textBuffer.pop_back();
                    parseTextBuffer();
                }
			break;
            case GLFW_KEY_ENTER: //Retour en avant dans les modifications manuelles
                if(freezeModifMode){
                    if(text != ""){
                        recuit->setTemp(parseTextBufferResult());
                    }
                    freezeModifMode = false;
                }
			break;
        }
    }else if (action == GLFW_RELEASE){
        if(key == GLFW_KEY_LEFT_CONTROL){ //Fin du mode déplacement de noeuds
            draggingViewMode = !draggingViewMode;
            draggingNodesMode = !draggingNodesMode;
            userAction = 0;
            changeCoord = true;
        }
    }
}

void parseTextBuffer(){
    stringstream ss;
    for(auto it = textBuffer.begin(); it != textBuffer.end(); it++){
    ss<< *it;
    }
    text = ss.str();
}
int parseTextBufferResult(){
    int result = 0;
    for (auto d : textBuffer)  
    {
        result = result * 10 + d;
    }
    return result;
}



/*---AFFICHAGE DES ELEMENTS DU GRAPHE---*/

//Affiche la boite englobante des points et emplacements du graphe
void openGLShowBoiteEnglobante() {
	glLineWidth(2.0f);
	glColor3f(0.4f, 0.4f, 0.4f); 
    Position<double> min = getPositionOrtho(xmin, ymin);
    Position<double> max = getPositionOrtho(xmax, ymax);

    glBegin(GL_LINE_STRIP);   
        glVertex2d(min.getX(), min.getY());
        glVertex2d(min.getX(), max.getY());
        glVertex2d(max.getX(), max.getY());
        glVertex2d(max.getX(), min.getY());
        glVertex2d(min.getX(), min.getY());
	glEnd();
}
//Affiche les noeuds du graphe
void openGLShowNodes(const Graphe& G) {
    glPointSize(8);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POINTS);
        for (int i = 0; i < G.getNbNoeuds(); i++) {
            Position<double> p = getPositionOrtho(G.getNoeud(i).getPos().getX(), G.getNoeud(i).getPos().getY());
            glVertex2d(p.getX(), p.getY());
        }
    glEnd();
}
//Affiche les emplacements du graphe
void openGLShowEmplacements(const Graphe& G) {
    glPointSize(8);
    glColor3f(0.0f, 1.0f, 0.0f);

    glBegin(GL_POINTS);  
        for (int i = 0; i < G.getNbEmplacements(); i++) {
            if (G.getEmplacement(i).estDisponible()) {
                Position<double> p = getPositionOrtho(G.getEmplacement(i).getPos().getX(), G.getEmplacement(i).getPos().getY());
                glVertex2d(p.getX(), p.getY());
            }
        }
    glEnd();
}
//Affiche les arrêtes du graphe, leur couleur dépend du type d'intersection et du mode d'affichage (touche I)
void openGLShowEdges(const Graphe& G) {
	glLineWidth(4.0f);
    for (int i = 0; i < G.getNbAretes(); i++) {
        Position<double> p1 = getPositionOrtho(G.getNoeud(G.getArete(i).getNoeudDep()).getPos().getX(), G.getNoeud(G.getArete(i).getNoeudDep()).getPos().getY());
        Position<double> p2 = getPositionOrtho(G.getNoeud(G.getArete(i).getNoeudArr()).getPos().getX(), G.getNoeud(G.getArete(i).getNoeudArr()).getPos().getY());

        if(G.getNbIntersectionsArete(i).nbSelf > 0 && showIllegalMode){
            glColor3f(0.3f, 0.2f, 0.5f);
        }else if(G.getNbIntersectionsArete(i).nbIllegales > 0 && showIllegalMode){
            glColor3f(1.0f, 0.0f, 1.0f);
        }else if(G.getNbIntersectionsArete(i).nbNormales > 0 && showAllIntersectionMode){
            glColor3f(0.75f, 0.0f, 0.0f);
        }else{
            //glColor3f(1.0f, 1.0f, 1.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
        }

        glBegin(GL_LINE_STRIP);
            glVertex2d(p1.getX(), p1.getY());
            glVertex2d(p2.getX(), p2.getY());
        glEnd();
    }
}
//Affiche la grille du graphe
void openGLShowCells(const Graphe& G) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 2.0f, 0.4f);
   
    Position<double> pmin = getPositionOrtho((double)G.getGrille().getXmin(), (double)G.getGrille().getYmin());
    Position<double> pmax = getPositionOrtho((double)(G.getGrille().getXmin() + G.getGrille().getLageurColonnes()*G.getGrille().getNbColonnes()), (double)(G.getGrille().getYmin() + G.getGrille().getHauteurLignes()*G.getGrille().getNbLignes()));

    for (int i=0;i<=G.getGrille().getNbLignes();i++) {
        Position<double> xy = getPositionOrtho((double)(G.getGrille().getXmin() + G.getGrille().getLageurColonnes()*i), (double)(G.getGrille().getYmin() + G.getGrille().getHauteurLignes()*i));
        glBegin(GL_LINE_STRIP);
            glVertex2d(pmin.getX(), xy.getY());
            glVertex2d(pmax.getX(), xy.getY());
        glEnd();

        glBegin(GL_LINE_STRIP);
            glVertex2d(xy.getX(), pmin.getY());
            glVertex2d(xy.getX(), pmax.getY());
        glEnd();
    } 
}
//Affiche les zones de selections autours des noeuds et emplacements
void openGLShowTargetCircles(const Graphe& G) {
    glColor3f(0.0f, 1.0f, 0.0f);		
    for (int i = 0; i < G.getNbEmplacements(); i++) {
        if (G.getEmplacement(i).estDisponible()) {
            Position<double> p = getPositionOrtho(G.getEmplacement(i).getPos().getX(), G.getEmplacement(i).getPos().getY());
            drawCircle((float)G.getEmplacement(i).getPos().getX(), (float)G.getEmplacement(i).getPos().getY(), hitbox);
        }
    }

    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < G.getNbNoeuds(); i++) {
        if (G.getNoeud(i).estPlace()) {
            Position<double> p = getPositionOrtho(G.getNoeud(i).getPos().getX(), G.getNoeud(i).getPos().getY());
            drawCircle((float)G.getNoeud(i).getPos().getX(), (float)G.getNoeud(i).getPos().getY(), hitbox);
        }
    }
}
//Dessine les cercles représentant les zones de selections autours des noeuds et emplacements
void drawCircle(float cx, float cy, double r, int num_segments){
    float theta = float(3.1415926) * 2 / float(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = float(r);//we start at angle = 0 
    float y = 0;
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
        Position<double> p = getPositionOrtho(cx, cy);
        glVertex2f((float)p.getX()+x, (float)p.getY()+y);//output vertex 

        //calculate the tangential std::vector 
        //remember, the radial std::vector is (x, y) 
        //to get the tangential std::vector we flip those coordinates and negate one of them 
        float tx = -y;
        float ty = x;

        //add the tangential std::vector  
        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor 
        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}
void openGLShowEverything(const Graphe& G) {
	//openGLShowBoiteEnglobante();
    if(G.hasGrid() && showCells)
        openGLShowCells(G);
    openGLShowEdges(G);
    openGLShowEmplacements(G);
    openGLShowNodes(G);
    if(showCircles)
        openGLShowTargetCircles(G);
}



void debugDisplay(Graphe& G){
    glBegin(GL_POINTS);
        glVertex2d(0.0f, 0.0f);
        Position<double> p = getPositionRepereActuelFromPixels(cursorPosX, cursorPosY);
        glVertex2d(p.getX(), p.getY());
        glVertex2d((xmax-xmin)/2, (ymax-ymin)/2);
        glVertex2d(0, 50);
        glVertex2d(90, 10);
        glVertex2d(frameWidth/2, frameHeight/2);
    glEnd();
}


/*---MAIN FUNCTION---*/
void dispOpenGL(Graphe& G, bool useReelCoord) {
    initVariablesGraphe(G);
    
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    GLFWwindow* window = initFenetre((int)frameOriginalWidth, (int)frameOriginalHeight, "Fenetre OpenGL");

    initRepere();
    while (!glfwWindowShouldClose(window)) {
		initFrame(window);
        repereOrtho(G);
        
        //debugDisplay(G);
        openGLShowEverything(G);

		glfwSwapBuffers(window);
        actionUtilisateurSurLeGraphe(G);
		glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}


void keyPressCheck(GLFWwindow*& window, GLFWwindow*& textRender, Graphe& G, RecuitSimule* r){
    bool init = false;
    #ifdef WIN32
        init = (GetAsyncKeyState(VK_SPACE) & 0x01) && !window;
    #endif
    #ifdef UNIX
        struct termios oldt, newt;
        int ch, oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);

        init = (ch == ' ') && !window;
    #endif
    if(init){
        window = initAffichage(G);
        textRender = initTextRender();
        recuit = r;         
    }
        
}

GLFWwindow* initFenetre(int w, int h, const char* name){
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    GLFWwindow* window = glfwCreateWindow(w, h, name, NULL, NULL);
    if (!window) { glfwTerminate(); exit(EXIT_FAILURE); }
    
    if(name != "Modifier la temperature"){
        glfwSetScrollCallback(window, mouse_scroll_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetWindowSizeCallback(window, window_size_callback);
    }
    glfwSetKeyCallback(window, key_callback);
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glfwMakeContextCurrent(window);
    glfwFocusWindow(window);
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    return window;
}

GLFWwindow* initAffichage(Graphe& G){
    initVariablesGraphe(G);   
    GLFWwindow* window = initFenetre((int)frameOriginalWidth, (int)frameOriginalHeight, "Fenetre OpenGL"); 
    initRepere();

    return window;
}
GLFWwindow* initTextRender(){
    GLFWwindow* window = initFenetre(500, 200, "Modifier la temperature"); 
    shader = nullptr;
    textRenderingInit(window, shader);
    return window;
}

void initFrame(GLFWwindow*& window){
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0, 0, frameWidth, frameHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    //glColor3f(1.0f, 1.0f, 1.0f);
    //glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();
}
void initFrameTextRender(GLFWwindow*& window){
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &frameRenderWidth, &frameRenderHeight);
    glViewport(0, 0, frameRenderWidth, frameRenderHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    //glColor3f(1.0f, 1.0f, 1.0f);
    //glColor3f(0.0f, 0.0f, 0.0f);
    glLoadIdentity();
}

void endAffichage(GLFWwindow*& window, GLFWwindow*& textRender){
    freezeModifMode = false;
    glfwDestroyWindow(window);
    glfwDestroyWindow(textRender);
    glfwTerminate();
    window = nullptr; 
    textRender = nullptr; 
    recuit = nullptr;
}

void frameModif(GLFWwindow*& window, GLFWwindow*& textRender, Graphe& G){  
    textBuffer = {};
    text = "";

    while(freezeModifMode){
        initFrame(window);
        repereOrtho(G);       
        openGLShowEverything(G);
        glfwSwapBuffers(window);
        actionUtilisateurSurLeGraphe(G);
        
        initFrameTextRender(textRender);
        displayTextModif(textRender, shader, text, to_string(recuit->getTemp()), to_string(recuit->getTempDep()));
        glfwSwapBuffers(textRender);
        glfwPollEvents();

        if(glfwWindowShouldClose(window) || glfwWindowShouldClose(textRender)){
            endAffichage(window, textRender);
        }
    }
    textBuffer = {};
    text = "";
}

void frameAffichage(GLFWwindow*& window, GLFWwindow*& textRender, Graphe& G){

    initFrame(window);
    repereOrtho(G);   
    openGLShowEverything(G);
    glfwSwapBuffers(window);

    initFrameTextRender(textRender);
    displayTextAffichage(textRender, shader, to_string(recuit->getTemp()), to_string(recuit->getTempDep()));
    glfwSwapBuffers(textRender);

    glfwPollEvents();
    
    if(glfwWindowShouldClose(window) || glfwWindowShouldClose(textRender)){
        endAffichage(window, textRender);
    }
    if(freezeModifMode){
        frameModif(window, textRender, G);
    }
}










 



