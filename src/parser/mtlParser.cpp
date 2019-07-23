#include "objModel.hpp"
#include <fstream>
#include <sstream>

mtlParser::mtlParser(std::string path) {
    std::ifstream is(path);
    std::ostringstream oss;
    oss << is.rdbuf(); //TODO: better way to file reading?
    is.close();
    std::string str = oss.str();

    strIter it = str.begin();
    strIter end = str.end();

    while (it != end) {
        if (*it == '\n' || *it == '\r')
            it++;
        else
            scanLine(it, end);
    }

    if (!curName.empty())
        this->mats.emplace(curName, curMaterial);
}

void mtlParser::scanLine(strIter &it, const strIter &end) {
    scanSpaces(it, end);
    if (checkWord(it, end, "newmtl"))
        scanNew(it, end);
    else if (*it == 'K') //Ka Kd Ks
        scanColor(it, end);
    else if (checkWord(it, end, "Ts") || *it == 'd')
        scanScalar(it, end);
    else if (checkWord(it, end, "Ns") || checkWord(it, end, "Ni"))
        scanScalar(it, end);
    else if (checkWord(it, end, "map_"))
        scanComment(it, end); //LATER
    else if (*it == '#')
        scanComment(it, end);
    else {
        //THROW
        scanComment(it, end);
    }
}

void mtlParser::scanNew(strIter &it, const strIter &end) {
    if (!curName.empty())
        this->mats.emplace(curName, curMaterial);
    
    it += strlen("newmtl");
    scanSpaces(it, end);
    curName = scanIdent(it, end);
    scanSpaces(it, end);

    curMaterial = Material();
}

void mtlParser::scanColor(strIter &it, const strIter &end) {
    it++;
    char prop = 0; // 1-Ambient 2-Diffuse 3-Specular
    if (*it == 'a')
        prop = 1;
    else if (*it == 'd')
        prop = 2;
    else if (*it == 's')
        prop = 3;
    else
        return;
    it++;
    scanSpaces(it, end);
    
    if (*it == 's' || *it == 'x')
        return; //Here is parsing only one type of the color input (spectral and XYZ is unsupported)

    float r = scanNumber(it, end);
    scanSpaces(it, end);
    float g = scanNumber(it, end);
    scanSpaces(it, end);
    float b = scanNumber(it, end);
    scanSpaces(it, end);

    if (prop == 1) //Ambient
        curMaterial.ambient_color = Color(r, g, b);
    if (prop == 2) //Diffuse
        curMaterial.diffuse_color = Color(r, g, b);
    if (prop == 3) //Specular
        curMaterial.specular_color = Color(r, g, b);
}

void mtlParser::scanScalar(strIter &it, const strIter &end) {
    char prop = 0;
    if (*it == 'd') {
        prop = 1; //Dissolve (d = 1 - Ts)
        it++;
    }
    else if (checkWord(it, end, "Ts")) {
        prop = 2; //Transparency
        it += strlen("Ts");
    }
    else if (checkWord(it, end, "Ns")) {
        prop = 3; //Specular exponent
        it += strlen("Ns");
    }
    else if (checkWord(it, end, "Ni")) {
        prop = 4; //Index of refraction (optical density)
        it += strlen("Ni");
    }
    else
        return;
    
    scanSpaces(it, end);
    float val = scanNumber(it, end);
    scanSpaces(it, end);

    if (prop == 1)
        curMaterial.albedo[1] = 1 - val;
    else if (prop == 2)
        curMaterial.albedo[1] = val;
    else if (prop == 3)
        curMaterial.specular_exponent = val;
    else if (prop == 4)
        curMaterial.refractive_index = val;
}