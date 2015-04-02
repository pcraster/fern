#if !defined(HPX_B808C8CA_810E_4583_9EA2_528553C8B511)
#define HPX_B808C8CA_810E_4583_9EA2_528553C8B511

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/components/server/managed_component_base.hpp>
#include <hpx/runtime/components/server/locking_hook.hpp>
#include <hpx/runtime/actions/component_action.hpp>


namespace examples { namespace server
{
    /// This class is a very simple example of an HPX component. An HPX
    /// component is a class that:
    ///
    ///     * Inherits from a component base class (either
    ///       \a hpx::components::managed_component_base or
    ///       \a hpx::components::simple_component_base).
    ///     * Exposes methods that can be called asynchronously and/or remotely.
    ///       These constructs are known as HPX actions.
    ///
    /// By deriving this component from \a locking_hook the runtime system 
    /// ensures that all action invocations are serialized. That means that 
    /// the system ensures that no two actions are invoked at the same time on
    /// a given component instance. This makes the component thread safe and no
    /// additional locking has to be implemented by the user.
    ///
    /// Components are first-class objects in HPX. This means that they are
    /// globally addressable; all components have a unique GID.
    ///
    /// This example demonstrates how to write a managed component. Managed
    /// components are allocated in bulk by HPX. When a component needs to be
    /// created in large quantities, managed components should be used. Because
    /// managed components are allocated in bulk, the creation of a new managed
    /// component usually does not require AGAS requests.
    ///
    /// This component exposes 3 different actions: reset, add and query.
    class managed_accumulator
      : public hpx::components::locking_hook<
            hpx::components::managed_component_base<managed_accumulator> 
        >
    {
    public:
        typedef boost::int64_t argument_type;

        managed_accumulator() : value_(0) {}

        // Exposed functionality of this component.

        /// Reset the value to 0.
        void reset()
        {
            // set value_ to 0.
            value_= 0;
        }

        /// Add the given number to the accumulator.
        void add(argument_type arg)
        {
            // add value_ to arg, and store the result in value_.
            value_ += arg;
        }

        /// Return the current value to the caller.
        argument_type query() const
        {
            // Get the value of value_.
            return value_;
        }

        // Each of the exposed functions needs to be encapsulated into an
        // action type, generating all required boilerplate code for threads,
        // serialization, etc.

        HPX_DEFINE_COMPONENT_ACTION(managed_accumulator, reset);
        HPX_DEFINE_COMPONENT_ACTION(managed_accumulator, add);
        HPX_DEFINE_COMPONENT_ACTION(managed_accumulator, query);

    private:
        argument_type value_;
    };
}}


HPX_REGISTER_ACTION_DECLARATION(
    examples::server::managed_accumulator::reset_action,
    managed_accumulator_reset_action)

HPX_REGISTER_ACTION_DECLARATION(
    examples::server::managed_accumulator::add_action,
    managed_accumulator_add_action)

HPX_REGISTER_ACTION_DECLARATION(
    examples::server::managed_accumulator::query_action,
    managed_accumulator_query_action)

#endif