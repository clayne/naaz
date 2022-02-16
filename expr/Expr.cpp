#include "Expr.hpp"

#include "../util/ioutil.hpp"
#include "../third_party/xxHash/xxh3.h"

namespace naaz::expr
{

uint64_t SymExpr::hash() const
{
    XXH64_state_t state;
    XXH64_reset(&state, 0);
    XXH64_update(&state, m_name.data(), m_name.size());
    XXH64_update(&state, &m_size, sizeof(m_size));
    return XXH64_digest(&state);
}

bool SymExpr::eq(ExprPtr other) const
{
    if (other->kind() != ekind)
        return false;

    std::shared_ptr<const SymExpr> other_ =
        std::static_pointer_cast<const SymExpr>(other);
    return m_name.compare(other_->m_name) == 0;
}

void SymExpr::pp() const { pp_stream() << m_name; }

uint64_t ConstExpr::hash() const
{
    XXH64_state_t state;
    XXH64_reset(&state, 0);
    XXH64_update(&state, &m_val, sizeof(m_val));
    XXH64_update(&state, &m_size, sizeof(m_size));
    return XXH64_digest(&state);
}

bool ConstExpr::eq(ExprPtr other) const
{
    if (other->kind() != ekind)
        return false;

    std::shared_ptr<const ConstExpr> other_ =
        std::static_pointer_cast<const ConstExpr>(other);
    return m_size == other_->m_size && m_val == other_->m_val;
}

void ConstExpr::pp() const
{
    // FIXME: print the number correctly
    pp_stream() << (uint64_t)m_val;
}

uint64_t AddExpr::hash() const
{
    XXH64_state_t state;
    XXH64_reset(&state, 0);
    XXH64_update(&state, &m_size, sizeof(m_size));
    void* raw_lhs = (void*)m_lhs.get();
    void* raw_rhs = (void*)m_rhs.get();
    XXH64_update(&state, raw_lhs, sizeof(void*));
    XXH64_update(&state, raw_rhs, sizeof(void*));
    return XXH64_digest(&state);
}

bool AddExpr::eq(ExprPtr other) const
{
    if (other->kind() != ekind)
        return false;

    std::shared_ptr<const AddExpr> other_ =
        std::static_pointer_cast<const AddExpr>(other);
    return m_size == other_->m_size && m_lhs.get() == other_->m_lhs.get() &&
           m_rhs.get() == other_->m_rhs.get();
}

void AddExpr::pp() const
{
    m_lhs->pp();
    pp_stream() << " + ";
    m_rhs->pp();
}

uint64_t SubExpr::hash() const
{
    XXH64_state_t state;
    XXH64_reset(&state, 0);
    XXH64_update(&state, &m_size, sizeof(m_size));
    void* raw_lhs = (void*)m_lhs.get();
    void* raw_rhs = (void*)m_rhs.get();
    XXH64_update(&state, raw_lhs, sizeof(void*));
    XXH64_update(&state, raw_rhs, sizeof(void*));
    return XXH64_digest(&state);
}

bool SubExpr::eq(ExprPtr other) const
{
    if (other->kind() != ekind)
        return false;

    std::shared_ptr<const SubExpr> other_ =
        std::static_pointer_cast<const SubExpr>(other);
    return m_size == other_->m_size && m_lhs.get() == other_->m_lhs.get() &&
           m_rhs.get() == other_->m_rhs.get();
}

void SubExpr::pp() const
{
    m_lhs->pp();
    pp_stream() << " - ";
    m_rhs->pp();
}

} // namespace naaz::expr