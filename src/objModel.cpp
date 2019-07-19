#include "objModel.hpp"

#include <fstream>
#include <sstream>

ObjModel::ObjModel(const Vec3f &center, const std::string &path) : center(center) {
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
        if (*it == '\n')
            it++;
        else
            scanLine(it, end);
    }
}

bool ObjModel::checkAssertion(bool expr, std::string msg) {
    return expr;
}

const std::vector<VolumeObj*>& ObjModel::getObjects() const {
    return this->objects;
}

//PARSER

void ObjModel::scanLine(strIter &it, const strIter &end) {
    scanSpaces(it, end);
    if (*it == '#')
        scanComment(it, end);
    else if (*it == 'v') //v vt vn (and vp)
        scanVertex(it, end);
    else if (*it == 'f')
        scanFace(it, end);
    else if (*it == 'm') //mtllib
        scanComment(it, end); //LATER
    else if (*it == 'u') //usemtl
        scanComment(it, end); //LATER
    else {
        //THROW
        scanComment(it, end);
    }
}

void ObjModel::scanVertex(strIter &it, const strIter &end) {
    it++; //Check bounds
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
        if (*it != '\n') {
            w = scanNumber(it, end);
            scanSpaces(it, end);
        }
        this->vs.push_back(Vec3f(x, y, z) + center);
    }
    else if (*it == 't') { //Texture vertex (x, y, [w])
        it++;
        float u, v, w = 0.f;
        scanSpaces(it, end);
        u = scanNumber(it, end);
        scanSpaces(it, end);
        v = scanNumber(it, end); //There the second coordinate is required (unlike the official structure)
        scanSpaces(it, end);
        if (*it != '\n') {
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

    while (it != end && *it != '\n') {
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
            objects.push_back(new nTriangle(
                Vertex(vs[vertices[0]], vns[normals[0]], vts[textures[0]]),
                Vertex(vs[vertices[i]], vns[normals[i]], vts[textures[i]]),
                Vertex(vs[vertices[i + 1]], vns[normals[i + 1]], vts[textures[i + 1]]),
                // vs[vertices[i]], vs[vertices[0]], vs[vertices[i + 1]],
                YELLOW_RUBBER //TEMPORARY
            ));
        }
        else {
            Vec3f norm = (vs[vertices[i]] - vs[vertices[0]]) ^ (vs[vertices[i + 1]] - vs[vertices[0]]);
            objects.push_back(new nTriangle(
                Vertex(vs[vertices[0]], norm, vts[textures[0]]),
                Vertex(vs[vertices[i]], norm, vts[textures[i]]),
                Vertex(vs[vertices[i + 1]], norm, vts[textures[i + 1]]),
                YELLOW_RUBBER //TEMPORARY
            ));
        }
    }
}

// NUMBER

bool isDigit(char ch) {
    return '0' <= ch && ch <= '9';
}

int ObjModel::scanWhole(strIter &it, const strIter &end) {
    checkAssertion(isDigit(*it), "Expected digit");
    int res = 0;
    while (it != end && isDigit(*it)) {
        res = res * 10 + (*it - '0');
        it++;
    }
    return res;
}

float ObjModel::scanFractPart(strIter &it, const strIter &end) {
    checkAssertion(isDigit(*it), "Expected digit");
    float res = 0.f;
    while (it != end && isDigit(*it)) {
        res = res * 0.1f + (*it - '0');
        it++;
    }
    return res * 0.1f;
}

int ObjModel::scanInt(strIter &it, const strIter &end) {
    int sign = 1;
    if (*it == '-')
        sign = -1;
    if (*it == '-' || *it == '+')
        it++;
    return sign * scanWhole(it, end);
}

float ObjModel::scanNumber(strIter &it, const strIter &end) {
    // ['+' | '-'] {0...9} ['.' {0...9} [('e' | 'E') [+ | -] {0...9}]]
    float sign = 1.f;
    if (*it == '-')
        sign = -1.f;
    if (*it == '-' || *it == '+')
        it++;
    float whole = scanWhole(it, end);
    float fract = 0.f;
    float exp = 1.f;
    bool hasExp = false;
    if (*it == '.') {
        it++;
        fract = scanFractPart(it, end);
    }
    if (*it == 'e' || *it == 'E') {
        hasExp = true;
        it++;
        float esign = 1.f;
        if (*it == '-')
            esign = -1.f;
        if (*it == '-' || *it == '+')
            it++;
        exp = esign * scanWhole(it, end);
    }
    float res = sign * (whole + fract);
    if (hasExp)
        powf(res, exp);
    return res;
}

// COMMENT AND SPACES

void ObjModel::scanComment(strIter &it, const strIter &end) {
    while (it != end && *it == '\n')
        it++;
    it++; //Newline
}

void ObjModel::scanSpaces(strIter &it, const strIter &end) {
    while (it != end && (*it == ' ' || *it == '\t' || *it == '\r'))
        it++;
}