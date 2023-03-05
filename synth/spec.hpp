#ifndef SPEC_H
#define SPEC_H

#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

#include "expr.hpp"

class Spec {
public:
    // Number of variables.
    const uint32_t num_vars;

    // Number of input/output examples.
    const uint32_t num_examples;

    // The i'th element is the name of the i'th variable.
    const std::vector<std::string> var_names;

    // The i'th element is the height of the i'th variable.
    const std::vector<int32_t> var_heights;

    // The i'th element specifies the values of the i'th variable,
    // where the j'th bit of that integer is the variable's value in example j.
    std::vector<uint32_t> var_values;

    // The i'th bit is the desired output in example i.
    uint32_t sol_result;

    // The example we are next going to replace
    int example_iter = 0;

    // The height of the solution circuit.
    const int32_t sol_height;

    std::vector<std::vector<bool>> all_inputs;

    std::vector<bool> all_sols;

    Spec(
        uint32_t num_vars,
        uint32_t num_examples,
        std::vector<std::string> var_names,
        std::vector<int32_t> var_heights,
        std::vector<uint32_t> var_values,
        uint32_t sol_result,
        int32_t sol_height,
        std::vector<std::vector<bool>> all_inputs,
        std::vector<bool> all_sols
    ) :
        num_vars(num_vars),
        num_examples(num_examples),
        var_names(var_names),
        var_heights(var_heights),
        var_values(var_values),
        sol_result(sol_result),
        sol_height(sol_height),
        all_inputs(all_inputs),
        all_sols(all_sols) {}

    void validate(const Expr* solution) {
        solution->assert_constant_height(sol_height, var_heights);
        for (uint32_t example = 0; example < num_examples; example++) {
            std::vector<bool> vars;
            for (uint32_t var = 0; var < num_vars; var++) {
                vars.push_back((var_values[var] >> example) & 1);
            }
            bool expected = (sol_result >> example) & 1;
	    //std::cout << "Validating: " << std::endl;
	    //std::cout << solution->eval(vars) << std::endl;
	    //std::cout << expected << std::endl;
            assert(solution->eval(vars) == expected);
        }
    }

    int advanceCEGISIteration(const Expr* solution) {
        int counter = counterexample(solution);
        if(counter == -1) return -1;
        for(uint32_t j=0; j<var_values.size(); j++) {
                var_values[j] &= ~(1<<example_iter);
                var_values[j] |= (all_inputs[counter][j]?1:0)<<example_iter;
        }
        sol_result &= ~(1<<example_iter);
        sol_result |= (all_sols[counter]?1:0)<<example_iter;
        example_iter = (example_iter + 1) % 32;
        return counter;
    }

    int counterexample(const Expr* solution) {
        for(uint32_t example=0; example<all_inputs.size(); example++) {
            if(solution->eval(all_inputs[example])!=all_sols[example]) {
                return example;
            }
        }
        return -1;
    }

    friend std::ostream& operator<< (std::ostream &out, const Spec &spec) {
        out << "num_vars: " << spec.num_vars
            << ", num_examples: " << spec.num_examples;

        out << ", var_names:";
        for (auto name : spec.var_names) {
            out << " " << name;
        }

        out << ", var_heights:";
        for (auto height : spec.var_heights) {
            out << " " << height;
        }

        out << ", var_values:";
        for (auto value : spec.var_values) {
            out << " " << std::bitset<32>(value);
        }

        out
            << ", sol_result: " << std::bitset<32>(spec.sol_result)
            << ", sol_height: " << spec.sol_height;

        return out;
    }

    void updateIOExamples(std::vector<uint32_t> updated_var_values, uint32_t updated_sol_result) {
        var_values = updated_var_values;
        sol_result = updated_sol_result;
    }
};

#endif
