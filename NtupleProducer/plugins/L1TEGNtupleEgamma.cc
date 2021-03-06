#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "L1TEGNtupleBase.h"

class L1TEGNtupleEgamma : public L1TEGNtupleBase {
public:
  L1TEGNtupleEgamma(const edm::ParameterSet& conf);
  ~L1TEGNtupleEgamma() override{};
  void initialize(TTree&, const edm::ParameterSet&, edm::ConsumesCollector&&) final;
  void fill(const edm::Event& e, const edm::EventSetup& es) final;

private:
  void clear() final;

  edm::EDGetToken egamma_token_;

  int egamma_n_;
  std::vector<float> egamma_pt_;
  std::vector<float> egamma_energy_;
  std::vector<float> egamma_eta_;
  std::vector<float> egamma_phi_;
  std::vector<float> egamma_hwQual_;
};

DEFINE_EDM_PLUGIN(HGCalTriggerNtupleFactory, L1TEGNtupleEgamma, "L1TEGNtupleEgamma");

L1TEGNtupleEgamma::L1TEGNtupleEgamma(const edm::ParameterSet& conf) : L1TEGNtupleBase(conf) {}

void L1TEGNtupleEgamma::initialize(TTree& tree, const edm::ParameterSet& conf, edm::ConsumesCollector&& collector) {
  egamma_token_ = collector.consumes<l1t::EGammaBxCollection>(conf.getParameter<edm::InputTag>("Egamma"));

  tree.Branch(branch_name_w_prefix("n").c_str(), &egamma_n_, branch_name_w_prefix("n/I").c_str());
  tree.Branch(branch_name_w_prefix("pt").c_str(), &egamma_pt_);
  tree.Branch(branch_name_w_prefix("energy").c_str(), &egamma_energy_);
  tree.Branch(branch_name_w_prefix("eta").c_str(), &egamma_eta_);
  tree.Branch(branch_name_w_prefix("phi").c_str(), &egamma_phi_);
  tree.Branch(branch_name_w_prefix("hwQual").c_str(), &egamma_hwQual_);
}

void L1TEGNtupleEgamma::fill(const edm::Event& e, const edm::EventSetup& es) {
  // retrieve towers
  edm::Handle<l1t::EGammaBxCollection> egamma_h;
  e.getByToken(egamma_token_, egamma_h);
  const l1t::EGammaBxCollection& egamma_collection = *egamma_h;

  // triggerTools_.eventSetup(es);

  clear();
  for (auto egee_itr = egamma_collection.begin(0); egee_itr != egamma_collection.end(0); egee_itr++) {
    egamma_n_++;
    // physical values
    // if(fabs(egee_itr->eta()) > 4) {
    //   std::cout << " eta: " << egee_itr->eta() << " HWqual: " << egee_itr->hwQual() << std::endl;
    // }
    egamma_pt_.emplace_back(egee_itr->pt());
    egamma_energy_.emplace_back(egee_itr->energy());
    egamma_eta_.emplace_back(egee_itr->eta());
    egamma_phi_.emplace_back(egee_itr->phi());
    egamma_hwQual_.emplace_back(egee_itr->hwQual());
  }
  // std::cout << branch_name_w_prefix("-") << " # EG: " << egamma_n_ << std::endl;
}

void L1TEGNtupleEgamma::clear() {
  egamma_n_ = 0;
  egamma_pt_.clear();
  egamma_energy_.clear();
  egamma_eta_.clear();
  egamma_phi_.clear();
  egamma_hwQual_.clear();
}
