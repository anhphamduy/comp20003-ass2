# comp20003-ass2

## Tree structure
The tree is built based on the median values as this will provide the most optimal search results.

## Insertion
- Keep two copies for the original list.
- Sort one copy by the x axis.
- Sort the other copy by y axis.

```python
def initialize_tree_from_list(list_sorted_by_x, list_sorted_by_y, start_index, end_index, is_x_axis):
    if start_index == end_index: return null

    node = Node

    if ix_x_axis:
        median = median of the list sorted by x 
    else:
        median = median of the list sorted by y

    assign the median value to the node value
    
    node -> left = initialize_tree_from_list(list_sorted_by_x, list_sorted_by_y, start_index, median, !is_x_axis)

    node -> right = initialize_tree_from_list(list_sorted_by_x, list_sorted_by_y, median_index, end_index, !is_x_axis)
    
    return node
```

## Sorting the linked list (Merge sort)

- Get the middle then sort in accordance with the left and right half of the linked list.

```python
def sort(head, comparison_func):
    if head == null or head.next == null: return head

    middle = get the middle of the list

    get the right half of the list
    
    first_half_sort_result = merge_sort(first_half_of the list)
    right_half_sort_result = merge_sort(right_half of the list)

    return merge(sorted first half, sorted right half, comparison_func)

def merge(list a, list b, comparison_func):
    go through the list a and list b in parallel
        compare element by element then put them into a new list by comparison_func

def get_middle(head):
    if head == null: return head

    have one counter to be the current position
    have another counter to be double the current position

    loop until the double counter reaches null
        update the two counter
        
    return the current position counter
```
