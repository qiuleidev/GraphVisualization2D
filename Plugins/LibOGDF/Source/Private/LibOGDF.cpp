#include "LibOGDFPrivatePCH.h"
#include "ogdf/basic/graph_generators.h"
#include "ogdf/layered/DfsAcyclicSubgraph.h"
#include "ogdf/fileformats/GraphIO.h"


class FLibOGDF : public ILibOGDF
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FLibOGDF, LibOGDF)

void FLibOGDF::StartupModule()
{


}

void FLibOGDF::ShutdownModule()
{
}
