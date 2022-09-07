#include "Path.h"

Path::Path() {

}

Path::Path(vector<int> path) {
    this->verticesVisited = path;
}

const vector<int> &Path::getVerticesVisited() const {
    return verticesVisited;
}

int Path::getLength() const {
    return length;
}

void Path::addVertex(int id, int length) {
    verticesVisited.push_back(id);
    this->length += length;
}

void Path::setLength(int length) {
    Path::length = length;
}

Path::Path(vector<int> path, int length) {
    this->verticesVisited = path;
    this->length = length;
}

// sluzy do usuwania pierwszego lub ostatniego odwiedzonego wierzcholka
void Path::removeVertex(int index, const vector<MyVertex> &vertices) {
    if (index == 0)
        length -= MyArc::calculateLength(vertices[verticesVisited[0]], vertices[verticesVisited[1]]);
    else if (index == verticesVisited.size() - 1)
        length -= MyArc::calculateLength(vertices[verticesVisited[verticesVisited.size() - 2]],
                                         vertices[verticesVisited[verticesVisited.size() - 1]]);
    else
        return;

    verticesVisited.erase(verticesVisited.begin() + index);
}
