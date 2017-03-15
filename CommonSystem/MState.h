#pragma once
//
// Created by islam on 11.03.17.
//

#include <cstdint>
#include <atomic>



/**
 * details. anonymous because
 */
namespace state_mixin_details {
    template<typename Enum>
    inline Enum operator&(Enum lhs, Enum rhs) {
        return static_cast<Enum>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

    template<typename Enum>
    inline Enum operator~(Enum arg) {
        return static_cast<Enum>(static_cast<int>(arg));
    }

    template<class StateType>
    class MGenericState {

        std::atomic<StateType> object_state;

    protected:
        StateType reset_state() {
            return set_state(StateType::CREATED);
        }

        StateType set_state(StateType state) {
            object_state = state;
            return get_state();
        }

        StateType add_state(StateType state) {
            object_state = object_state.load() & state;
            return get_state();
        }

        StateType remove_state(StateType state) {
            object_state = object_state.load() & ~state;
            return get_state();
        }

    public:
        MGenericState() : object_state(StateType::CREATED) {}

        StateType get_state() const {
            return object_state;
        }

        bool has_state(StateType state) const {
            return static_cast<bool>(object_state.load() & state);
        }
    };
}

enum class State: uint8_t {
    CREATED     = 0b00000001,
    INITIALIZED = 0b00000010
};

enum class ActivityState: uint8_t {
    CREATED     = 0b00000001,
    INITIALIZED = 0b00000010,
    RUNNING     = 0b00000100
};

typedef state_mixin_details::MGenericState<State>            MState;
typedef state_mixin_details::MGenericState<ActivityState>    MActivityState;

