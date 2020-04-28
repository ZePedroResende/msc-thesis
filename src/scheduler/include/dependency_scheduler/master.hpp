//
// Created by resende on 4/26/20.
//

#ifndef TEG_MASTER_HPP
#define TEG_MASTER_HPP

#include <atomic>
#include <data.hpp>
#include <utility>
#include <set>

#include "scheduler/queue.hpp"
#include "teg.h"
#include "teg_dependency.hpp"
#include "util.hpp"

namespace dependency_scheduler {

    void master(const std::shared_ptr<scheduler::Queue<int>> &q,
                const std::shared_ptr<scheduler::Queue<std::pair<int, int>>> &r,
                const std::shared_ptr<std::vector<std::shared_ptr<Data>>> &data_vec) {
        auto cache = build_result_cache();

        auto next_up = std::queue<int>();


        static int initializer = 2;

        int current = 2;

        for (auto &next: get_no_deps_fns(initializer)) {
            auto pair = std::make_pair(next, -2);
            update_cache(&cache, &pair);
            q->push(&next);
        }

        while (!data_vec->empty()) {
            std::pair<int, int> *pair = r->next();
            int next = pair->second;
            // E necessario adicionar os que ja estao a ser processados
            if (current != pair->first) {
                update_cache(&cache, pair);
            } else {
                next = get_cache(&cache, pair->second);
            }

            if (next == TEG::FAIL) {
                data_vec->erase(data_vec->begin());

                next = 2;
            }

            if (next == TEG::SUCCESS) {
                std::cout << "TRUE\n";
                data_vec->erase(data_vec->begin());
                // save data
                next = 2;
                cache = build_result_cache();
                current = 2;
            }

            if (!data_vec->empty()) {
                for (auto &n: get_no_deps_fns(next)) {
                    if (n != -2) q->push(&n);
                }
            }

        }
        q->stop();
    }
}  // namespace dependency_scheduler
#endif //TEG_MASTER_HPP
