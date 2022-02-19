#pragma once

#include <set>
#include <memory>

#include "MapMemory.hpp"
#include "../loader/AddressSpace.hpp"
#include "../lifter/PCodeLifter.hpp"
#include "../expr/Expr.hpp"

namespace naaz::state
{

class State;
typedef std::shared_ptr<State> StatePtr;
class State
{
    uint64_t m_pc;

    std::unique_ptr<MapMemory> m_regs;
    std::unique_ptr<MapMemory> m_ram;

    std::shared_ptr<loader::AddressSpace> m_as;
    std::shared_ptr<lifter::PCodeLifter>  m_lifter;

    std::set<expr::ExprPtr> m_constraints;

  public:
    State(std::shared_ptr<loader::AddressSpace> as,
          std::shared_ptr<lifter::PCodeLifter> lifter, uint64_t pc)
        : m_as(as), m_lifter(lifter), m_pc(pc)
    {
        m_regs = std::unique_ptr<MapMemory>(new MapMemory());
        m_ram  = std::unique_ptr<MapMemory>(new MapMemory(as.get()));
    }

    State(const State& other);
    ~State() {}

    const Arch& arch() const { return m_lifter->arch(); }

    bool get_code_at(uint64_t addr, uint8_t** o_data, uint64_t* o_size);

    expr::ExprPtr read(expr::ExprPtr addr, size_t len);
    expr::ExprPtr read(uint64_t addr, size_t len);
    void          write(expr::ExprPtr addr, expr::ExprPtr data);
    void          write(uint64_t addr, expr::ExprPtr data);

    expr::ExprPtr reg_read(const std::string& name);
    expr::ExprPtr reg_read(uint64_t offset, size_t size);
    void          reg_write(const std::string& name, expr::ExprPtr data);
    void          reg_write(uint64_t offset, expr::ExprPtr data);

    void                           add_constraint(expr::ExprPtr c);
    const std::set<expr::ExprPtr>& pi() const { return m_constraints; }

    void     set_pc(uint64_t pc) { m_pc = pc; }
    uint64_t pc() const { return m_pc; }

    StatePtr clone() const { return std::shared_ptr<State>(new State(*this)); }
};

} // namespace naaz::state