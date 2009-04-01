{

float tot_numEvents_selected = 0;
float tot_numEvents_analyzed = 0;

TNtuple* m_ntuple = (TNtuple*) _file0->Get("/SkimEfficiency/ntuple");
float numEvents_selected;
m_ntuple->SetBranchAddress("numEvents_selected",&numEvents_selected);
float numEvents_analyzed;
m_ntuple->SetBranchAddress("numEvents_analyzed",&numEvents_analyzed);
for (int i =0;i<m_ntuple->GetEntries();++i){m_ntuple->GetEntry(i); tot_numEvents_selected+=numEvents_selected;  tot_numEvents_analyzed+=numEvents_analyzed;}

std::cerr << "tot_numEvents_selected = " << tot_numEvents_selected << std::endl;
std::cerr << "tot_numEvents_analyzed = " << tot_numEvents_analyzed << std::endl;
std::cerr << "efficiency = " << tot_numEvents_selected/tot_numEvents_analyzed << std::endl;


std::cerr << "|  " << tot_numEvents_selected;
std::cerr << "  |  " << tot_numEvents_analyzed;
std::cerr << "  |  " << tot_numEvents_selected/tot_numEvents_analyzed << "  |  " << std::endl;

}