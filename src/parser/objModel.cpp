#include "objModel.hpp"
#include <fstream>
#include <sstream>

ObjModel::ObjModel(const Vec3f &center, const Mat3f &trans, const std::string &path) : 
        center(center), transform(trans) {
    int rootl = path.size();
    auto trIt = path.rbegin();
    while (*trIt != '/' && *trIt != '\\') { //Crutch?
        rootl--;
        trIt++;
    }
    auto tIt = path.begin();
    std::ostringstream rootss;
    for (int i = 0; i < rootl; i++) {
        rootss << *tIt;
        tIt++;
    }
    root = rootss.str();

    std::ifstream is(path);
    std::ostringstream oss;
    oss << is.rdbuf(); //TODO: better way to file reading?
    is.close();
    std::string str = oss.str();

    strIter it = str.begin();
    strIter end = str.end();

    vs.push_back(this->center);
    vts.push_back(Vec2f(0, 0));
    vns.push_back(Vec3f(0, 0, 0));

    while (it != end) {
        if (*it == '\n' || *it == '\r')
            it++;
        else
            scanLine(it, end);
    }
}

bool ObjModel::checkAssertion(bool expr, std::string msg) {
    return expr;
}


void ObjModel::scanLine(strIter &it, const strIter &end) {
    scanSpaces(it, end);
    if (*it == 'v') //v vt vn (and vp)
        scanVertex(it, end);
    else if (*it == 'f')
        scanFace(it, end);
    else if (checkWord(it, end, "mtllib"))
        scanMtl(it, end);
    else if (checkWord(it, end, "usemtl"))
        scanMtl(it, end);
    else if (*it == '#')
        scanComment(it, end);
    else {
        //THROW
        scanComment(it, end);
    }
}

void ObjModel::scanVertex(strIter &it, const strIter &end) {
    it++;
    if (*it == ' ' || *it == '\t') { //Vertex (x, y, z, [w])
        it++;
        float x, y, z, w = 1.0f;
        scanSpaces(it, end);
        x = scanNumber(it, end);
        scanSpaces(it, end);
        y = scanNumber(it, end);
        scanSpaces(it, end);
        z = scanNumber(it, end);
        scanSpaces(it, end);
        if (*it != '\n' && *it != '#') {
            w = scanNumber(it, end);
            scanSpaces(it, end);
        }
        this->vs.push_back(transform * Vec3f(x, y, z) + center);
    }
    else if (*it == 't') { //Texture vertex (x, y, [w])
        it++;
        float u, v, w = 0.f;
        scanSpaces(it, end);
        u = scanNumber(it, end);
        scanSpaces(it, end);
        v = scanNumber(it, end); //There the second coordinate is required (unlike the official structure)
        scanSpaces(it, end);
        if (*it != '\n' && *it != '#') {
            w = scanNumber(it, end);
            scanSpaces(it, end);
        }
        this->vts.push_back(Vec2f(u, v));
    }
    else if (*it == 'n') { //Normal (x, y, z)
        it++;
        float x, y, z;
        scanSpaces(it, end);
        x = scanNumber(it, end);
        scanSpaces(it, end);
        y = scanNumber(it, end);
        scanSpaces(it, end);
        z = scanNumber(it, end);
        scanSpaces(it, end);
        this->vns.push_back(Vec3f(x, y, z));
    }
    else {
        //THROW
        scanComment(it, end);
    }
}

void ObjModel::scanFace(strIter &it, const strIter &end) {
    std::vector<int> vertices;
    std::vector<int> textures;
    std::vector<int> normals;
    bool hasT = false, hasN = false;
    it++;
    scanSpaces(it, end);

    //Start scaning first vertex
    vertices.push_back(scanInt(it, end));
    scanSpaces(it, end);
    if (*it == '/') {
        it++;
        scanSpaces(it, end);
        if (*it == '/') {
            it++;
            hasT = false;
            hasN = true;
            textures.push_back(0);
            normals.push_back(scanInt(it, end));
            //End with (V _ N)
        }
        else {
            hasT = true;
            textures.push_back(scanInt(it, end));
            scanSpaces(it, end);
            if (*it == '/') {
                it++;
                hasN = true;
                normals.push_back(scanInt(it, end));
                //End with (V T N)
            }
            else {
                hasN = false;
                normals.push_back(0);
                //End with (V T _)
            }
        }
    }
    else {
        hasT = false;
        hasN = false;
        textures.push_back(0);
        normals.push_back(0);
        //End with (V _ _)
    }
    scanSpaces(it, end);

    while (it != end && *it != '\n' && *it != '#') {
        vertices.push_back(scanInt(it, end));
        scanSpaces(it, end);
        if (hasT) {
            it++; // '/'
            scanSpaces(it, end);
            textures.push_back(scanInt(it, end));
            scanSpaces(it, end);
        }
        else
            textures.push_back(0);
        if (hasN) {
            if (!hasT) {
                it++; // '/'
                scanSpaces(it, end);
            }
            it++; // '/'
            scanSpaces(it, end);
            normals.push_back(scanInt(it, end));
            scanSpaces(it, end);
        }
        else
            normals.push_back(0);
    }
    
    for (int i = 1; i < vertices.size() - 1; i++) {
        if (hasN) {
            objects.push_back(new Triangle(
                Vertex(vs[vertices[0]], vns[normals[0]], vts[textures[0]]),
                Vertex(vs[vertices[i]], vns[normals[i]], vts[textures[i]]),
                Vertex(vs[vertices[i + 1]], vns[normals[i + 1]], vts[textures[i + 1]]),
                curMat
            ));
        }
        else {
            Vec3f norm = (vs[vertices[i]] - vs[vertices[0]]) ^ (vs[vertices[i + 1]] - vs[vertices[0]]);
            objects.push_back(new Triangle(
                Vertex(vs[vertices[0]], norm, vts[textures[0]]),
                Vertex(vs[vertices[i]], norm, vts[textures[i]]),
                Vertex(vs[vertices[i + 1]], norm, vts[textures[i + 1]]),
                curMat
            ));
        }
    }
}

void ObjModel::scanMtl(strIter &it, const strIter &end) {
    char prop = 0;
    if (*it == 'm') {
        prop = 1;
        it += strlen("mtllib");
    }
    else if (*it == 'u') {
        prop = 2;
        it += strlen("usemtl");
    }
    scanSpaces(it, end);
    std::string ident = scanIdent(it, end);
    scanSpaces(it, end);
    if (prop == 1) {
        mtlParser parser(root + ident);
        for (auto&& e : parser.mats)
            mats.emplace(e);
    }
    else if (prop == 2) {
        auto t = mats.find(ident);
        curMat = t != mats.end() ? t->second : YELLOW_RUBBER;
    }
}