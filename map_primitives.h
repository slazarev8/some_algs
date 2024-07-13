#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

enum class EEdgeType { EForward, EBackWard, EBidir };

struct TEdge {
    int Id;
    bool RotationAllowed;
    EEdgeType Type;
};

enum class EVertexType { ECommonNode, ESemaphore, ECross };

struct TVertex {
    EVertexType type;
    int id;
};

struct TPoint2D {
    double X;
    double Y;
};

class TWHMap {
public:
    TWHMap(std::vector<TEdge> edges, std::vector<TVertex> vertexes)
        : Edges_(std::move(edges))
        , Vertexes_(std::move(vertexes)) {
    }

    const std::vector<TEdge>& GetEdges() const {
        return Edges_;
    }

    std::vector<TEdge>& GetEdges() {
        return Edges_;
    }

    const std::vector<TVertex>& GetVertexes() const {
        return Vertexes_;
    }

    std::vector<TVertex>& GetVertexes() {
        return Vertexes_;
    }

    std::vector<TPoint2D> GetPath(std::vector<int> idPath) {
        std::vector<TPoint2D> result;
        return result;
    }

    int GetNearestVertex(TPoint2D point) const {
        // Find nearest ind
        return 0;
    }

private:
    std::vector<TEdge> Edges_;
    std::vector<TVertex> Vertexes_;
};

struct TPath {
    std::vector<int> Path;
};

class IPathFinder {
public:
    virtual TPath FindPath(TPoint2D from, TPoint2D to) const = 0;
    virtual ~IPathFinder() = default;
};

class TDijkstra: public IPathFinder {
public:
    TDijkstra(std::shared_ptr<TWHMap> map)
        : Map_(std::move(map)){};

    TPath FindPath(TPoint2D from, TPoint2D to) const override {
        TPath result;
        const auto fromId = Map_->GetNearestVertex(from);
        const auto toId = Map_->GetNearestVertex(to);
        return result;
    }

private:
    std::shared_ptr<const TWHMap> Map_;
};

class ISemaphore {
public:
    virtual bool Block(const std::vector<int> pathIds, int nextIndId) = 0;
    virtual void Release(int id) = 0;

private:
};

class TCommonSemaphore: public ISemaphore {
public:
    virtual bool Block(const std::vector<int> pathIds, int nextIndId) {
        if (Locked_)
            return false;
        Locked_ = true;
        return true;
    }

    virtual void Release(int id) {
        Locked_ = false;
    }

private:
    bool Locked_;
};

class TChainSignal: public ISemaphore {
public:
    virtual bool Block(const std::vector < std::shared_ptr<ISemaphore> pathSemaphores,
                       int nextIndId) {
        return pathSemaphores[nextIndId + 1]->IsBlock();
    }
};

class TSemaphoreBlocker {
public:
    bool Block(const std::vector<int> pathIds, int nextIndId) {
        if (!Semaphores_.count(nextIndId))
            return true;
        return Semaphores_[nextIndId].Block();
    }

private:
    // std::shared_ptr<const TWHMap> Map_;
    std::unordered_map<int, ISemaphore> Semaphores_;
};

int main() {
    TWHMap map({}, {});
    auto& data = map.GetEdges();
}
