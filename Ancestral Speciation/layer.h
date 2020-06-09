#pragma once
#include "node.h"
/// <summary>
/// a layer of nodes used as
/// the intermediate data structure
/// in forward propagation.
/// </summary>
struct layer
{
	layer();
	~layer();

	/// <summary>
	/// add incoming nodes to buffer array
	/// </summary>
	/// <param name="incoming"></param>
	/// <param name="incoming_size"></param>
	void update(node** &incoming, int incoming_size);

	/// <summary>
	/// remove the given node from buffer
	/// </summary>
	/// <param name="removal"></param>
	void remove(node* removal);

	/// <summary>
	/// return a copy of the buffer for freezing a layer
	///	during forward propagation.
	/// </summary>
	/// <returns></returns>
	node** copy_buffer();

	/// <summary>
	/// checks if this layer only contains outputs.
	/// </summary>
	/// <returns></returns>
	bool final_layer(node** &check, int check_size);

	node** buffer;
	int buffer_size = 0;
};

