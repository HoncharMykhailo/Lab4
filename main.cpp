#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <string>

using namespace std;

struct Automaton {
    set<int> states;                            // Set of states
    set<char> alphabet;                         // Set of alphabet symbols
    int initialState;                           // Initial state
    set<int> finalStates;                       // Set of final states
    map<int, vector<pair<char, int>>> transitions;  // Transition function
};

void readAutomaton(Automaton& automaton) {
    int numStates, numAlphabet, numFinalStates;

    cout << "Enter number of states: ";
    cin >> numStates;
    cout << "Enter states (as integers): ";
    for (int i = 0; i < numStates; ++i) {
        int state;
        cin >> state;
        automaton.states.insert(state);
    }

    cout << "Enter number of symbols in the alphabet: ";
    cin >> numAlphabet;
    cout << "Enter symbols (as characters): ";
    for (int i = 0; i < numAlphabet; ++i) {
        char symbol;
        cin >> symbol;
        automaton.alphabet.insert(symbol);
    }

    cout << "Enter the initial state: ";
    cin >> automaton.initialState;
    if (automaton.states.find(automaton.initialState) == automaton.states.end()) {
        cout << "Error: Initial state is not in the set of states!" << endl;
        return;
    }

    cout << "Enter number of final states: ";
    cin >> numFinalStates;
    cout << "Enter final states: ";
    for (int i = 0; i < numFinalStates; ++i) {
        int finalState;
        cin >> finalState;
        if (automaton.states.find(finalState) != automaton.states.end()) {
            automaton.finalStates.insert(finalState);
        } else {
            cout << "Error: State " << finalState << " is not in the set of states!" << endl;
        }
    }

    int numTransitions;
    cout << "Enter number of transitions: ";
    cin >> numTransitions;
    cout << "Enter transitions (format: current_state symbol next_state):" << endl;
    for (int i = 0; i < numTransitions; ++i) {
        int currentState, nextState;
        char symbol;
        cin >> currentState >> symbol >> nextState;

        // Check if currentState and nextState belong to states, and symbol belongs to alphabet
        if (automaton.states.find(currentState) == automaton.states.end()) {
            cout << "Error: State " << currentState << " is not in the set of states!" << endl;
            continue;
        }
        if (automaton.states.find(nextState) == automaton.states.end()) {
            cout << "Error: State " << nextState << " is not in the set of states!" << endl;
            continue;
        }
        if (automaton.alphabet.find(symbol) == automaton.alphabet.end()) {
            cout << "Error: Symbol " << symbol << " is not in the alphabet!" << endl;
            continue;
        }

        automaton.transitions[currentState].push_back({symbol, nextState});
    }
}

void findReachableStates(const Automaton& automaton, vector<bool>& reachable) {
    vector<int> stateStack;
    stateStack.push_back(automaton.initialState);
    reachable[automaton.initialState] = true;

    while (!stateStack.empty()) {
        int current = stateStack.back();
        stateStack.pop_back();

        bool hasTransitions = false;
        for (const auto& [state, transitions] : automaton.transitions) {
            for (const auto& [symbol, nextState] : transitions) {
              //  cout << state << " --(" << symbol << ")--> " << nextState << endl;
                if(state == automaton.transitions.find(current)->first)
                {
                    hasTransitions = true;
                    break;
                }
            }
            if(hasTransitions) break;
        }
        if(!hasTransitions)
        {
            cout << "State " << current << " has 0 transitions." << endl;
            continue;
        }



        if (automaton.transitions.find(current) != automaton.transitions.end()) {
            const vector<pair<char, int>>& transitionsFromCurrent = automaton.transitions.at(current);

            for (size_t i = 0; i < transitionsFromCurrent.size(); ++i) {
                int nextState = transitionsFromCurrent[i].second;

                if (!reachable[nextState]) {
                    reachable[nextState] = true;
                    stateStack.push_back(nextState);
                }
            }

            cout<<"stack:\n";
            cout<<"isEmpty = "<<stateStack.empty()<<"\n";
            cout<<"size = "<<stateStack.size()<<"\n";
            for(int i = 0;i<stateStack.size();i++)
            {
                cout<<stateStack[i]<<" ";
            }
            cout<<"\n";


        } else {
            cout << "State " << current << " has no transitions." << endl;
        }



    }
}


bool canReachFinalState(int state, const Automaton& automaton, set<int>& visited) {
    if (visited.find(state) != visited.end()) {
        return false;
    }
    visited.insert(state);

    if (automaton.finalStates.find(state) != automaton.finalStates.end()) {
        return true;
    }

    if (automaton.transitions.find(state) != automaton.transitions.end()) {
        for (const auto& transition : automaton.transitions.at(state)) {
            int nextState = transition.second;
            if (canReachFinalState(nextState, automaton, visited)) {
                return true;
            }
        }
    }

    return false;
}


set<int> findDeadStates(const Automaton& automaton) {
    set<int> deadStates; // Set to store dead states

    for (int state : automaton.states) {
        if (automaton.finalStates.find(state) != automaton.finalStates.end()) {
            continue;
        }

        if (automaton.transitions.find(state) == automaton.transitions.end()) {
            deadStates.insert(state);
            continue;
        }

        set<int> visited;
        if (!canReachFinalState(state, automaton, visited)) {
            deadStates.insert(state);
        }
    }

    return deadStates;
}



int main() {
    Automaton automaton;

    readAutomaton(automaton);

    vector<bool> reachable(automaton.states.size(), false);

    findReachableStates(automaton, reachable);

    cout << "Unreachable states: ";
    for (int state : automaton.states) {
        if (!reachable[state]) {
            cout << state << " ";
        }
    }

    set<int> deadStates = findDeadStates(automaton);

    cout << "\nDead states: ";
    for (int deadState : deadStates) {
        cout << deadState << " ";
    }
    cout << endl;




    cout << endl;

    return 0;
}
