#pragma once
#include "quantum_to_llvm.hpp"

namespace qcor {
// The goal of InstOpLowering is to convert all QuantumDialect
// InstOp (quantum.inst) to the corresponding __quantum__qis__INST(int64*, ...)
// call
class InstOpLowering : public ConversionPattern {
protected:
  // Symbol table, local seen variables
  std::map<std::string, mlir::Value> &variables;

  // Mapping of Vector::ExtractElementOp Operation pointers to the
  // corresponding qreg variable name they represent
  std::map<mlir::Operation *, std::string> &qubit_extract_map;

  std::vector<std::string> &module_function_names;
  mutable std::map<std::string, std::string> inst_map{{"cx", "cnot"},
                                                      {"measure", "mz"}};

public:
  // The Constructor, store the variables and qubit extract op map
  explicit InstOpLowering(MLIRContext *context,
                          std::map<std::string, mlir::Value> &vars,
                          std::map<mlir::Operation *, std::string> &qem,
                          std::vector<std::string> &f_names)
      : ConversionPattern(mlir::quantum::InstOp::getOperationName(), 1,
                          context),
        variables(vars), qubit_extract_map(qem),
        module_function_names(f_names) {}

  // Match and replace all InstOps
  LogicalResult
  matchAndRewrite(Operation *op, ArrayRef<Value> operands,
                  ConversionPatternRewriter &rewriter) const override;
};
} // namespace qcor