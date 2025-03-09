
#include <signals\signals.hpp>

#include <string>

/**
 * A Delegate holds function callbacks.
 */

// Primary delegate template.
template <typename Func>
class Delegate;

template <typename R, typename... Args>
class Delegate<R(Args...)>
{
public:
    using slot              = sig::slot<R(Args...)>;
    using signal            = sig::signal<R(Args...)>;
    using connection        = sig::connection;
    using scoped_connection = sig::scoped_connection;

    /**
     * Add function callback to the delegate.
     *
     * @param f The function to add to the delegate.
     * @returns The connection object that can be used to remove the callback
     * from the delegate.
     */
    template <typename Func>
    connection operator+=(Func&& f)
    {
        return m_Callbacks.connect(std::forward<Func>(f));
    }

    /**
     * Remove a callback function from the delegate.
     *
     * @param f The function to remove from the delegate.
     * @returns The number of callback functions removed.
     */
    template <typename Func>
    std::size_t operator-=(Func&& f)
    {
        return m_Callbacks.disconnect(std::forward<Func>(f));
    }

    /**
     * Invoke the delegate.
     */
    opt::optional<R> operator()(Args... args) { return m_Callbacks(std::forward<Args>(args)...); }

private:
    signal m_Callbacks;
};