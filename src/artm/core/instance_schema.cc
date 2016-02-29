// Copyright 2014, Additive Regularization of Topic Models.

#include "artm/core/instance_schema.h"

#include <sstream>

#include "artm/regularizer_interface.h"
#include "artm/score_calculator_interface.h"
#include "artm/messages.pb.h"

namespace artm {
namespace core {

InstanceSchema::InstanceSchema()
    : regularizers_(), score_calculators_() {}

InstanceSchema::InstanceSchema(const InstanceSchema& schema)
    : regularizers_(schema.regularizers_),
      score_calculators_(schema.score_calculators_) {}

std::shared_ptr<InstanceSchema> InstanceSchema::Duplicate() const {
  return std::shared_ptr<InstanceSchema>(new InstanceSchema(*this));
}

void InstanceSchema::RequestMasterComponentInfo(MasterComponentInfo* master_info) const {
  for (auto& regularizer : regularizers_) {
    MasterComponentInfo::RegularizerInfo* info = master_info->add_regularizer();
    info->set_name(regularizer.first);
    info->set_type(typeid(*regularizer.second).name());
  }

  for (auto& score : score_calculators_) {
    MasterComponentInfo::ScoreInfo* info = master_info->add_score();
    info->set_name(score.first);
    info->set_type(typeid(*score.second).name());
  }
}

void InstanceSchema::set_regularizer(const std::string& name,
                                     const std::shared_ptr<RegularizerInterface>& regularizer) {
  auto iter = regularizers_.find(name);
  if (iter != regularizers_.end()) {
    iter->second = regularizer;
  } else {
    regularizers_.insert(std::make_pair(name, regularizer));
  }
}

bool InstanceSchema::has_regularizer(const std::string& name) const {
  auto iter = regularizers_.find(name);
  return iter != regularizers_.end();
}

void InstanceSchema::clear_regularizer(const std::string& name) {
  auto iter = regularizers_.find(name);
  if (iter != regularizers_.end()) {
    regularizers_.erase(iter);
  }
}

std::shared_ptr<std::vector<std::string> > InstanceSchema::regularizers_list() {
  auto retval = std::make_shared<std::vector<std::string>>(std::vector<std::string>());
  for (auto iter = regularizers_.begin(); iter != regularizers_.end(); ++iter)
    retval->push_back(iter->first);

  return retval;
}

std::shared_ptr<RegularizerInterface> InstanceSchema::regularizer(const std::string& name) const {
  auto iter = regularizers_.find(name);
  if (iter != regularizers_.end()) {
    return iter->second;
  } else {
    return nullptr;
  }
}

void InstanceSchema::set_score_calculator(
    const ScoreName& name,
    const std::shared_ptr<ScoreCalculatorInterface>& score_calculator) {
  auto iter = score_calculators_.find(name);
  if (iter != score_calculators_.end()) {
    iter->second = score_calculator;
  } else {
    score_calculators_.insert(std::make_pair(name, score_calculator));
  }
}

bool InstanceSchema::has_score_calculator(const ScoreName& name) const {
  auto iter = score_calculators_.find(name);
  return iter != score_calculators_.end();
}

void InstanceSchema::clear_score_calculator(const ScoreName& name) {
  auto iter = score_calculators_.find(name);
  if (iter != score_calculators_.end()) {
    score_calculators_.erase(iter);
  }
}

std::shared_ptr<ScoreCalculatorInterface> InstanceSchema::score_calculator(
    const ScoreName& name) const {
  auto iter = score_calculators_.find(name);
  if (iter != score_calculators_.end()) {
    return iter->second;
  } else {
    return nullptr;
  }
}

void InstanceSchema::clear_score_calculators() {
  score_calculators_.clear();
}

}  // namespace core
}  // namespace artm
