#include "composed_string_view.hxx"

#include <experimental/string_view>
#include <string>
#include <memory>

#include <iostream>

using namespace std;
using namespace std::experimental;

namespace csv
{
template <typename CHAR_T, typename LEFT, typename RIGHT>
struct composed_string_view
{
    LEFT m_left;
    RIGHT m_right;

    using this_t = composed_string_view<CHAR_T, LEFT, RIGHT>;

    template <typename CONT> struct const_iterator_base
    {
        const CONT* m_view;
        enum ACTIVE
        {
            LEFT_ACTIVE,
            RIGHT_ACTIVE
        };

        ACTIVE m_active;

        union
        {
            typename LEFT::const_iterator m_subItLeft;
            typename RIGHT::const_iterator m_subItRight;
        };

        const_iterator_base(const CONT* view, ACTIVE active,
                            typename LEFT::const_iterator subItLeft,
                            typename RIGHT::const_iterator subItRight)
            : m_view(view), m_active(active)
        {
            if (m_active == LEFT_ACTIVE)
                m_subItLeft = subItLeft;
            else
                m_subItRight = subItRight;
        }

        const_iterator_base(const const_iterator_base& other)
            : m_view(other.m_view), m_active(other.m_active)
        {
            if (m_active == LEFT_ACTIVE)
                m_subItLeft = other.m_subItLeft;
            else
                m_subItRight = other.m_subItRight;
        }

        const_iterator_base& operator=(const const_iterator_base& other)
        {
            m_view = other.m_view;
            m_active = other.m_active;

            if (m_active == LEFT_ACTIVE)
                m_subItLeft = other.m_subItLeft;
            else
                m_subItRight = other.m_subItRight;

            return *this;
        }


        const_iterator_base& operator++()
        {
            if (m_active == LEFT_ACTIVE)
            {
                if (m_subItLeft == m_view->m_left.end())
                {
                    m_active = RIGHT_ACTIVE;
                    m_subItRight = m_view->m_right.begin();
                }
                else
                    ++m_subItLeft;
            }
            else
                ++m_subItRight;

            return *this;
        }

        const_iterator_base operator++(int)
        {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        bool equalSubIt(const const_iterator_base& other) const
        {
            bool result = m_active == other.m_active &&
                          (m_active == LEFT_ACTIVE
                               ? (m_subItLeft == other.m_subItLeft)
                               : (m_subItRight == other.m_subItRight));
            return result;
        }

        bool operator==(const const_iterator_base& other) const
        {
            bool result = m_view == other.m_view;
            result = result && equalSubIt(other);
            return result;
        }

        bool operator!=(const const_iterator_base& other) const
        {
            return !(*this == other);
        }

        const CHAR_T& operator*() const
        {
            return m_active == LEFT_ACTIVE ? *m_subItLeft : *m_subItRight;
        }
    };

    using const_iterator = const_iterator_base<this_t>;

    const_iterator begin() const
    {
        return const_iterator(this, const_iterator::LEFT_ACTIVE, m_left.begin(),
                              m_right.end());
    }

    const_iterator end() const
    {
        return const_iterator(this, const_iterator::RIGHT_ACTIVE,
                              m_left.begin(), m_right.end());
    }

    size_t size() const { return m_left.size() + m_right.size(); }

    operator std::string() const
    {
        std::string result(" ", size() + 1);
        auto it = std::copy(m_left.begin(), m_left.end(), result.begin());
        std::copy(m_right.begin(), m_right.end(), it);
        return result;
    }
};

} // namespace csv

using namespace csv;

int main(int argc, const char** argv)
{
    string_view one("Hallo ");
    string_view two("Welt");
    string_view three(", wie");

    using type_one = composed_string_view<char, string_view, string_view>;
    type_one view_one{one, two};

    cout << "size: " << view_one.size() << endl;

    using type_two = composed_string_view<char, type_one, string_view>;
    type_two viewTwo{view_one, three};

    cout << "size two: " << viewTwo.size() << endl;
    string test = viewTwo;
    cout << "Test: " << test << endl;

    cout << "Size *void: " << sizeof(void*) << endl;
    cout << "Size strView one: " << sizeof(one) << endl;
    cout << "Size one: " << sizeof(view_one) << endl;
    cout << "Size: " << sizeof(viewTwo) << endl;
    cout << "Size it_view: " << sizeof(string_view::const_iterator) << endl;
    cout << "Size it_one: " << sizeof(type_one::const_iterator) << endl;
    cout << "Size it_two: " << sizeof(type_two::const_iterator) << endl;

    return 0;
}
