#include "tester.h"

Tester::Tester() : Tester(100){ }

Tester::Tester(int n_) : n(n_) {
    insertBatchSize = 10;
    searchBatchSize = 100;
    removeBatchSize = 1000;
    knnBatchSize = 1000;
    polygonLimit = 5000;
}

void Tester::insert() {
    Rtree * r = new Rtree();
        for(int e = 0; e < n; e++) {
            std::vector<double> times;
            for(int i = 0; i < polygonLimit/insertBatchSize; i++) {
                times.push_back(insertBatch(r));
            }
            double avg = 0;
            for(auto t : times) {
                avg += t;
            }
            printf("Tiempo promedio del experimento: %d ms", avg/(polygonLimit/insertBatchSize));
        }
}

void Tester::search() {
    Rtree * r = new Rtree();
    for(int e = 0; e < n; e++) {
        std::vector<double> times;
        for(int i = 0; i < polygonLimit/insertBatchSize; i++) {
            times.push_back(searchBatch(r));
        }
        double avg = 0;
        for(auto t : times) {
            avg += t;
        }
        printf("Tiempo promedio del experimento: %d ms", avg/(polygonLimit/searchBatchSize));
    }
}

void Tester::remove(){
    Rtree * r = new Rtree();
    for(int e = 0; e < n; e++) {
        std::vector<double> times;
        std::vector<Figure> figs;
        for(int i = 0; i < polygonLimit; i++) {
                Figure f = Display::generateRandomFigure();
                figs.push_back(f);
            }
            for(int i = 0; i < polygonLimit/removeBatchSize; i++) {
                times.push_back(removeBatch(r, figs));
            }
            double avg = 0;
            for(auto t : times) {
                avg += t;
            }
            printf("Tiempo promedio del experimento: %d ms\n", avg/(polygonLimit/removeBatchSize));
        }
}

void Tester::knn() {
    Rtree * r = new Rtree();
    for(int k = 0; i < kTestsSize; k++) {
        for(int e = 0; e < n; e++) {
            std::vector<double> times;
            for(int i = 0; i < polygonLimit/knnBatchSize; i++) {
                times.push_back(knnBatch(r, kTests[i]));
            }
            double avg = 0;
            for(auto t : times) {
                avg += t;
            }
            printf("Tiempo promedio del experimento: %d ms", avg/(polygonLimit/searchBatchSize));
        }
    }
}

double Tester::insertBatch(Rtree* &r) {
            std::vector<Figure> figs;
            for(int i = 0; i < insertBatchSize; i++) {
                auto f = Display::generateRandomFigure();
                figs.push_back(f);
            }
            start = clock();
            for(auto f : figs) {
                r->insert(f);
            }
            end = clock();
            printf("Tiempo total: %d ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
            return ((double)(end-start))*1000/CLOCKS_PER_SEC;
        }

double Tester::searchBatch(Rtree* &r) {
        std::vector<Figure> figs;
    for(int i = 0; i < searchBatchSize; i++) {
        Figure f = Display::generateRandomFigure();
        figs.push_back(f);
        r->insert(f);
    }
    start = clock();
    for(auto f : figs) {
        r->search(f.getCentroidPoint());
    }
    end = clock();
    printf("Tiempo total: %d ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
    return ((double)(end-start))*1000/CLOCKS_PER_SEC;
}

double Tester::removeBatch(Rtree* &r, &std::vector<Figure>) {
    start = clock();
    for(auto f : figs) {
        r->remove(f.getCentroidPoint());
    }
    end = clock();
    printf("Tiempo total: %d ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
    return ((double)(end-start))*1000/CLOCKS_PER_SEC;
}

double Tester::knnBatch(Rtree* &r, int k) {
    std::vector<Figure> figs;
    for(int i = 0; i < searchBatchSize; i++) {
        Figure f = Display::generateRandomFigure();
        figs.push_back(f);
        r->insert(f);
    }
    start = clock();
    for(auto f : figs) {
        r->depthFirst(f.getCentroidPoint(), k);
    }
    end = clock();
    printf("Tiempo total: %d ms\n", ((double)(end-start))*1000/CLOCKS_PER_SEC);
    return ((double)(end-start))*1000/CLOCKS_PER_SEC;
}