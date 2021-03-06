/*!
 *  Copyright (c) 2016 by Contributors
 * \file graph.h
 * \brief Utilities to get information about schedule graph.
 */
#ifndef SCHEDULE_GRAPH_H_
#define SCHEDULE_GRAPH_H_

#include <tvm/expr.h>
#include <tvm/operation.h>
#include <tvm/schedule.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace TVM {
namespace schedule {

/*!
 * \brief data structure of Operation->Tensors it reads
 */
using ReadGraph = Map<Operation, Array<Tensor> >;

/*!
 * \brief AttachPath maps op-> a list of IterVar
 */
using AttachPath = Map<Operation, Array<IterVar> >;

/*!
 * \brief The map between tensor and operation it feeds to.
 */
using FeedGraph = std::unordered_map<Tensor, std::vector<Operation> >;

/*!
 * \brief Get read graph of each operation to all the
 *  Tensors that it directly depends on.
 *
 *  The result map contains Operations needed to finish root Operation.
 * \param roots The root operation.
 * \return The result map.
 */
ReadGraph CreateReadGraph(const Array<Operation>& roots, const Schedule& sch);

/*!
 * \brief Get minimum subgraph between outputs and inputs.
 *  The operations contains node which input-reachable from any inputs
 *  output reachable to any outputs.
 *
 *  The inputs won't be included in the subgraph, the outputs will be included.
 *
 * \param outputs The outputs of the subgraph
 * \param inputs The inputs to the subgraph.
 * \param include_inputs Whether to include inputs
 *
 * \return The subgraph.
 */
Array<Operation> GetSubGraph(const Array<Tensor>& outputs,
                             const Array<Tensor>& inputs, bool include_inputs);

/*!
 * \brief Get a post DFS ordered of operations in the graph.
 * \param roots The root of the graph.
 * \param g The read graph.
 * \return vector order of Operations in PostDFS order.
 *
 * \note PostDFSOrder is a special case of Topoligical order,
 *   and can be used when topoligical order is needed.
 */
Array<Operation> PostDFSOrder(const Array<Operation>& roots,
                              const ReadGraph& g);

/*!
 * \brief Create feedgraph for given Schedule
 * \param  g The read graph.
 * \return The created feedgraph.
 */
FeedGraph CreateFeedGraph(const ReadGraph& g);

/*!
 * \brief Create AttachPath that  maps op-> a list of IterVar
 *  That represents the loop nest op sits in from inner most to outermost
 *  Also inserts attach_stage for scan updates when needed.
 *
 * \param sch The schedule.
 * \return The attach path.
 */
AttachPath CreateAttachPath(Schedule sch);
}  // namespace schedule
}  // namespace TVM

#endif  // SCHEDULE_GRAPH_H_
