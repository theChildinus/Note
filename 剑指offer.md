# 剑指Offer

### Q： 替换空格

```java
// 从前往后替换
public String replaceSpace(StringBuffer str) {
    if (str == null) {
        return null;
    }

    StringBuffer ans = new StringBuffer();
    for (int i = 0; i < str.length(); i++) {
        if (str.charAt(i) != ' ') {
            ans.append(str.charAt(i));
        } else {
            ans.append('%');
            ans.append('2');
            ans.append('0');
        }
    }
    return ans.toString();
}
```

```java
// 从后向前替换
public String replaceSpace(StringBuffer str) {
    int spacenum = 0;//spacenum为计算空格数
    for(int i=0;i<str.length();i++){
        if(str.charAt(i)==' ')
            spacenum++;
    }
    int indexold = str.length()-1; //indexold为为替换前的str下标
    int newlength = str.length() + spacenum*2;//计算空格转换成%20之后的str长度
    int indexnew = newlength-1;//indexold为为把空格替换为%20后的str下标
    // 要先求出下标后再setLength 否则下标会有问题
    str.setLength(newlength);//使str的长度扩大到转换成%20之后的长度,防止下标越界
    for(;indexold>=0 && indexold<newlength;--indexold){ 
            if(str.charAt(indexold) == ' '){  //
            str.setCharAt(indexnew--, '0');
            str.setCharAt(indexnew--, '2');
            str.setCharAt(indexnew--, '%');
            }else{
                str.setCharAt(indexnew--, str.charAt(indexold));
            }
    }
    return str.toString();
}
```

### Q： 倒序打印链表

思路：ArrayList缓冲，从前往后遍历添加

栈

递归

```java
ArrayList<Integer> arrayList=new ArrayList<Integer>();
public ArrayList<Integer> printListFromTailToHead(ListNode listNode) {
    if(listNode!=null){
        this.printListFromTailToHead(listNode.next);
        arrayList.add(listNode.val);
    }
    return arrayList;
}
```

### Q： 重建二叉树

根据前序遍历和中序遍历重建二叉树

```java
public TreeNode reConstructBinaryTree(int [] pre, int [] in) {
    TreeNode root=reConstructBinaryTree(pre, 0, pre.length-1, in, 0, in.length-1);
    return root;
}
//前序遍历{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}
private TreeNode reConstructBinaryTree(int [] pre, int startPre, int endPre, int [] in, int startIn, int endIn) {

    if(startPre > endPre || startIn > endIn)
        return null;
    TreeNode root = new TreeNode(pre[startPre]);

    for(int i = startIn; i <= endIn; i++) // 在中序遍历中查找根节点的位置
        if(in[i] == pre[startPre]) { // 需要中序遍历中的某个值 和 前序遍历中 startPre 位置的值相等
            root.left = reConstructBinaryTree(pre, startPre+1, startPre+(i-startIn), in, startIn, i-1);
            root.right = reConstructBinaryTree(pre, startPre+(i-startIn)+1, endPre, in, i+1, endIn);
                    break;
        }

    return root;
}
```

### Q： 两个栈实现队列

```java
Stack<Integer> stack1 = new Stack<Integer>();
Stack<Integer> stack2 = new Stack<Integer>();

public void push(int node) {
    stack1.push(node);
}

// 弹出元素时 如果stack2不为空，不必再压到stack1中
// 因为就算此时有新元素到达 出队的元素仍然是stack2栈顶的元素
public int pop() {
    if (stack2.isEmpty()) {
        while (!stack1.isEmpty()) {
            stack2.push(stack1.pop());
        }
    }

    int ret = stack2.pop();
    return ret;
}
```

> 如果题目要求在排序的数组中查找一个数字或者统计某个数字出现的次数，我们都可以尝试使用二分查找算法

### Q： 旋转数组中的最小数字

二分查找，目标元素位于前后两个递增数组交界的地方

第一个指针指向第一个元素，第二个指针指向最后一个元素

- 中间元素位于前面的递增子数组，那么它应该大于或者等于第一个指针指向的元素，我们可以让第一个指针指向中间元素
- 中间元素位于后面的递增子数组，那么他应该小于或者等于最后一个指针指向的元素，我们可以让第二个指针指向中间元素

查找范围缩小到原来的一半之后，最终第一个指针会指向前面子数组的最后一个元素，第二个指针会指向后面子数组的第一个元素，即目标元素

特殊情况：

- 数组默认已经排好序
- 含有重复元素 `[1, 0, 1, 1, 1]`, `[1, 1, 1, 0, 1]` 无法分别中间数字 `1` 是位于第一个递增子数组还是位于第二个递增子数组

```java
public int minNumberInRotateArray(int [] array) {
    if (array.length <= 0) {
        return 0;
    }
    int low = 0;
    int high = array.length - 1;
    int mid = low; // 对于第一种特殊情况 初始化mid为low 当数组默认排好序 不会进入下面的循环
    while (array[low] >= array[high]) {
        if (low + 1 == high) {
            mid = high;
            break;
        }
        mid = (low + high) / 2;
        if (array[low] == array[mid] && array[mid] == array[high]) {
            // 对于第二种特殊情况 顺序遍历数组 寻找最小元素
            return minInOrder(array, low, high);
        }
        if (array[mid] > array[high]) {
            low = mid;
        }
        else if (array[mid] <= array[high]) {
            high = mid;
        }
    }
    return array[mid];
}

public int minInOrder(int[] array, int low, int high) {
    int ret = array[low];
    for (int i = low + 1; i <= high; i++) {
        if (ret > array[i]) {
            ret = array[i];
        }
    }
    return ret;
}
```

### Q： 斐波那契数列

```java
public int Fibonacci(int n) {
    int[] result = new int[2];
    result[0] = 0;
    result[1] = 1;
    if (n <= 1) {
        return result[n];
    }
    int first = 0;
    int second = 1;
    int ans = 0;
    for (int i = 2; i <= n; i++) {
        ans = first + second;
        first = second;
        second = ans;
    }
    return ans;
}
```

### Q： 青蛙跳台阶

```txt
一只青蛙一次可以跳上1级台阶，也可以跳上2级。求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。
```

`f(n) = f(n - 1) + f(n - 2)`

```java
public int JumpFloor(int target) {
    int a = 0;
    int b = 1;
    if (target == 0) {
        return 0;
    }
    if (target == 1) {
        return 1;
    }
    if (target == 2) {
        return 2;
    }
    while (target-- > 0) {
        b = a + b;
        a = b - a;
    }
    return b;
}
```

```txt
改变条件：一只青蛙一次可以跳上1级台阶，也可以跳上2级……它也可以跳上n级。求该青蛙跳上一个n级的台阶总共有多少种跳法。
```

`f(n) = f(n - 1) + f(n - 2) + ... + f(2) + f(1) + 1 = 2^(n-1)`

分别对应 跳1阶、跳2阶、... 跳n - 2阶、跳n - 1阶、跳n阶的跳法

### Q： 矩阵覆盖

```txt
我们可以用2*1的小矩形横着或者竖着去覆盖更大的矩形。请问用n个2*1的小矩形无重叠地覆盖一个2*n的大矩形，总共有多少种方法？
```

假设 `2*n` 矩阵有 `f(n)` 种方法

- `2*1` 的小矩阵竖着放时，剩余的矩阵还有 `f(n - 1)` 种放法
- `2*1` 的小矩阵横着放时，下面的 `1*2` 部分只能横着被填充，所有剩余部分还有 `f(n - 2)` 种放法

`f(n) = f(n - 1) + f(n - 2)`

### Q： 二进制中的1的个数

把一个整数减去1，和原整数做与运算，会把该整数最右边一个1编程0，那么一个整数的二进制表示有多少个1，就可以进行多少次这样的操作

```java
public int NumberOf1(int n) {
    int count = 0;
    while (n != 0) {
        ++count;
        n = ((n - 1) & n);
    }
    return count;
}
```

### Q： 数值的整数次方

由于计算机表示小数有误差，所以我们不能直接使用 `==` 符号判断两个小数是否相等，如果两个小数的差的绝对值很小，比如小于 `0.0000001`，就可以认为它们相等

```java
public double Power(double base, int exponent) {
    if ((base - 0.0 > -0.0000001) && (base - 0.0 < 0.0000001) && exponent < 0) {
        return 0.0;
    }
    int tmpexp = exponent;
    if (exponent < 0) {
        tmpexp = -exponent;
    }
    double result = 1.0;
    for (int i = 0; i < tmpexp; i++) {
        result *= base;
    }
    if (exponent < 0) {
        result = 1.0 / result;
    }
    return result;
}
```

简单快速幂方法

```java
/**
 * 1.全面考察指数的正负、底数是否为零等情况。
 * 2.写出指数的二进制表达，例如13表达为二进制1101。
 * 3.举例:10^1101 = 10^0001*10^0100*10^1000。
 * 4.通过&1和>>1来逐位读取1101，为1时将该位代表的乘数累乘到最终结果。
 */
public double Power(double base, int exponent) {
    double res = 1.0, curr = base;
    int n = exponent;
    if (n < 0) {
        if (base == 0)
            throw new RuntimeException("分母不能为0");
        exponent = (-exponent);
    } else if (n == 0) {// n==0
        return 1;// 0的0次方
    }
    while (exponent != 0) {
        if((exponent & 1) == 1)
            res *= curr;
        curr *= curr;// 翻倍
        exponent >>= 1;// 右移一位
    }
    return n >= 0 ? res : (1 / res);
}
```

### Q： 打印1到最大的n位数

```txt
输入3，打印1,2,3... 一直到最大的3位数999
```

面试小提示：如果面试题是关于n位整数并且没有限定n的取值范围，或者是输入任意大小的整数，那么这个题目很有可能需要考虑大数问题，字符串是一个简单、有效表示大数的方法

n位所有十进制数其实就是n个从 0-9 的全排列

```java
//打印1到最大的n位数的主方法
public void printToMaxOfDigits(int n) {
    if(n <= 0){
        System.out.println("输入的n没有意义");
        return;
    }
    char number[] = new char[n];
    for (int i = 0; i < number.length; i++) {
        number[i] = '0';
    }
    for (int i = 0; i < 10; ++i) {
        number[0] = (char) (i + '0');
        printToMaxOfNDigitsRecursively(number, n, 0);
    }
}

//利用递归实现1到最大的n位数的全排列
public void printToMaxOfNDigitsRecursively(char[] number, int n, int index) {
    if(index == n - 1){
        printNumber(number);
        return;
    }
    for (int i = 0; i < 10; ++i) {
        number[index + 1] = (char) (i + '0');
        printToMaxOfNDigitsRecursively(number, n, index + 1);
    }
}

//输出
private void printNumber(char[] number) {
    boolean isBeginning0 = true;
    int nLength = number.length;
    for (int i = 0; i < nLength; ++i) {
        if(isBeginning0 && number[i]!='0'){
            isBeginning0 = false;
        }
        if(!isBeginning0){
            System.out.print(number[i]);
        }
    }
    System.out.println();
}
```

### Q： 在 `O(1)` 时间删除链表节点

时间复杂度为 `[(n - 1) * O(1) + O(n)] / n = O(1)`

```java
void deletedelNode(Node head, Node delNode) {
    if (head == null || delNode== null)
        return;
    if (head == delNode) // 要删除的节点只有一个，删除头结点
        head = null;
    else if (delNode.next != null) { //要删除的节点不是尾节点
        delNode.value = delNode.next.value;
        delNode.next = delNode.next.next;
    } else { //链表中有多个节点 删除尾节点
        Node node1 = head;
        while (node1.next.next != null) {
            node1 = node1.next;
        }
        // 停在尾节点的前一个节点
        node1.next = null;
    }
}
```

### Q： 链表中倒数第k个节点

代码写的要鲁棒性高

```txt
第一个指针 0    n-k+1（即倒数第k个）
第二个指针 k-1    n
```

```java
public ListNode FindKthToTail(ListNode head,int k) {
    if (head == null || k == 0) {
        return null;
    }
    ListNode first = head; // 后行指针
    ListNode second = head; // 先行指针
    for (int i = 0; i < k - 1; i++) {
        if (second.next != null) {
            second = second.next; // 先行指针向后移动k-1个位置
        } else {
            return null; // 此时链表中节点个数小于k
        }

    }
    while (second.next != null) { // 两个指针同时向后移动
        first = first.next;
        second = second.next;
    }
    return first;
}
```

当我们用一个指针遍历链表不能解决问题的时候，可以尝试用两个指针来遍历链表

可以让其中一个指针遍历的速度快一些（比如一次在链表上走两步），或者让它先在链表上走若干步

### Q： 反转链表

```java
public ListNode ReverseList(ListNode head) {
    ListNode pre = null;
    ListNode cur = head;
    while (cur != null) {
        ListNode next = cur.next;
        cur.next = pre;
        pre = cur;
        cur = next;
    }
    return pre;
}
```

递归：从链表尾部开始改变节点指针方向

```java
public ListNode ReverseList(ListNode head) {
    if (head == null || head.next == null) return head; // 递归调用到最后一层即递归到尾节点时，在这里已经return了
    ListNode p = ReverseList(head.next);
    head.next.next = head;
    head.next = null;
    return p;
}
```

### Q： 合并两个排序的链表

```java
public ListNode Merge(ListNode list1,ListNode list2) {
    if (list1 == null) {
        return list2;
    } else if (list2 == null) {
        return list1;
    }

    ListNode newHead = null;
    if (list1.val < list2.val) {
        newHead = list1;
        newHead.next = Merge(list1.next, list2);
    } else {
        newHead = list2;
        newHead.next = Merge(list1, list2.next);
    }
    return newHead;
}
```

### Q： 树的子结构

输入两棵二叉树A，B，判断B是不是A的子结构。（ps：我们约定空树不是任意一个树的子结构）

```java
public boolean HasSubtree(TreeNode root1,TreeNode root2) {
    if (root1 == null || root2 == null) {
        return false;
    }
    return isSame(root1, root2) || HasSubtree(root1.left, root2) || HasSubtree(root1.right, root2);
}

public boolean isSame(TreeNode root1, TreeNode root2) {
    if (root2 == null) return true;
    if (root1 == null) return false;
    if (root1.val == root2.val) {
        return isSame(root1.left, root2.left) && isSame(root1.right, root2.right);
    } else {
        return false;
    }
}
```

### Q： 二叉树的镜像

```java
public void Mirror(TreeNode root) {
    if (root == null) {
        return;
    }
    if (root.left == null && root.right == null) {
        return;
    }
    TreeNode tmp = root.left;
    root.left = root.right;
    root.right = tmp;
    if (root.left != null) {
        Mirror(root.left);
    }
    if (root.right != null) {
        Mirror(root.right);
    }
}
```

### Q： 栈的压入、弹出序列

```java
public boolean IsPopOrder(int [] pushA,int [] popA) {
    if (pushA.length == 0 || popA.length == 0) {
        return false;
    }

    Stack<Integer> stack = new Stack<Integer>();
    int j = 0;
    for (int i = 0; i < pushA.length; i++) {
        stack.push(pushA[i]);
        while (j < popA.length && stack.peek() == popA[j]) {
            stack.pop();
            j++;
        }

    }
    return stack.empty() ? true : false;
}
```

### Q： 二叉搜索树的后序遍历序列

输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。

```java
public boolean VerifySquenceOfBST(int [] sequence) {
    if (sequence.length == 0) {
        return false;
    }
    int len = sequence.length;
    return helper(array, 0, len - 1);
}

public boolean helper(int[] array, int start, int end) {
    if (start >= end) {
        return true;
    }
    int i = start;
    while (array[i] < array[end]) {
        i++;
    }
    for (int j = i; j < end; j++) {
        if (array[j] < array[end]) {
            return false;
        }
    }
    return helper(array, start, i - 1) && helper(array, i, end - 1);
}
```

### Q： 复杂链表的复制

思路：第一步复制原链表上的每一个节点用next连接起来，第二部复制每个节点上的random指针

优化定位random指针：

1. map保存 (N, N’) 的对应关系
2. 在每个节点之后添加复制节点，再复制random指针，最后拆分链表

```java
public RandomListNode Clone(RandomListNode pHead)
{
    CloneNodes(pHead);
    SetRandomNode(pHead);
    return DivideList(pHead);
}

public void CloneNodes(RandomListNode pHead) {
    if (pHead == null) return;
    RandomListNode pNode = pHead;
    while (pNode != null) {
        RandomListNode pCloned = new RandomListNode(0);
        pCloned.label = pNode.label;
        pCloned.next = pNode.next;
        pCloned.random = null;

        pNode.next = pCloned;
        pNode = pCloned.next;
    }
}

public void SetRandomNode(RandomListNode pHead) {
    if (pHead == null) return;
    RandomListNode pNode = pHead;
    while (pNode != null) {
        RandomListNode pCloned = pNode.next;
        if (pNode.random != null) {
            pCloned.random = pNode.random.next;
        }
        pNode = pCloned.next;
    }
}

public RandomListNode DivideList(RandomListNode pHead) {
    if (pHead == null) return null;
    RandomListNode pNode = pHead;
    RandomListNode pClonedHead = null;
    RandomListNode pClonedNode = null;
    if (pNode != null) {
        pClonedHead = pClonedNode = pNode.next;
        pNode.next = pClonedNode.next;
        pNode = pClonedNode.next; 
    }
    while (pNode != null) {
        pClonedNode.next = pNode.next;
        pClonedNode = pClonedNode.next;
        pNode.next = pClonedNode.next;
        pNode = pNode.next;
    }
    return pClonedHead;
}
```

### Q： 二叉搜索树和双向链表

非递归：

```java
public TreeNode Convert(TreeNode pRootOfTree) {
    if (pRootOfTree == null) {
        return null;
    }

    Stack<TreeNode> stack = new Stack<TreeNode>();
    TreeNode p = pRootOfTree;
    TreeNode pre = null;
    TreeNode retRoot = null;
    boolean isFirst = true;
    while (!stack.isEmpty() || p != null) {
        if (p != null) {
            stack.push(p);
            p = p.left;
        } else {
            TreeNode node = stack.pop();
            if (isFirst) {
                retRoot = node;
                pre = retRoot;
                isFirst = false;
            } else {
                pre.right = node;
                node.left = pre;
                pre = node;
            }
            p = node.right;
        }
    }
    return retRoot;
}
```

递归：

```java
TreeNode list = null;
public TreeNode Convert(TreeNode pRootOfTree) {
    if(pRootOfTree == null) return pRootOfTree;

    Convert(pRootOfTree.right);
    if(list == null){
        list = pRootOfTree;
    } else {
        list.left = pRootOfTree;
        pRootOfTree.right = list;
        list = pRootOfTree;
    }
    Convert(pRootOfTree.left);

    return list;
}
```

### Q： 字符串的排列

无重复值的情况

- 固定第一个字符，递归取得首位后面的各种字符串组合；
- 再把第一个字符与后面每一个字符交换，并同样递归获得首位后面的字符串组合； 
- 递归的出口，就是只剩一个字的时候，递归的循环过程，就是从每个子串的第二个字符开始依次与第一个字符交换，然后继续处理子串。

有重复值的情况

由于全排列就是从第一个数字起，每个数分别与它后面的数字交换，我们先尝试加个这样的判断——如果一个数与后的数字相同那么这两个数就不交换了。

例如abb，第一个数与后面两个数交换得bab，bba。然后abb中第二个数和第三个数相同，就不用交换了。
但是对bab，第二个数和第三个数不 同，则需要交换，得到bba。
由于这里的bba和开始第一个数与第三个数交换的结果相同了，因此这个方法不行。

- 换种思维，对abb，第一个数a与第二个数b交换得到bab，然后考虑第一个数与第三个数交换，此时由于第三个数等第二个数， 所以第一个数就不再用与第三个数交换了。
- 再考虑bab，它的第二个数与第三个数交换可以解决bba。此时全排列生完毕！

```java
public ArrayList<String> Permutation(String str) {
    List<String> res = new ArrayList<>();
    if (str != null && str.length() > 0) {
        PermutationHelper(str.toCharArray(), 0, res);
        Collections.sort(res);
    }
    return (ArrayList)res;
}

public void PermutationHelper(char[] cs, int i, List<String> list) {
    if (i == cs.length - 1) {
        String val = String.valueOf(cs);
        if (!list.contains(val))
            list.add(val);
    } else {
        for (int j = i; j < cs.length; j++) {
            swap(cs, i, j);
            PermutationHelper(cs, i+1, list);
            swap(cs, i, j);
        }
    }
}

public void swap(char[] cs, int i, int j) {
    char temp = cs[i];
    cs[i] = cs[j];
    cs[j] = temp;
}
```

### Q： 数组中出现次数超过一半的数字

思路：

- partition 寻找 数组的中位数
- count计数，当遇到相同的数字 count加一，不相同时减一，count等于0更换数字

```java

```

### Q： 连续子数组的最大和

```java
public int FindGreatestSumOfSubArray(int[] array) {
    if (array.length == 0) {
        return 0;
    }
    int result = array[0];
    int sum = 0;
    for (int i = 0; i < array.length; i++) {
        sum = Math.max(sum + array[i], array[i]);
        result = Math.max(sum, result);
    }
    return result;
}

DP:

public int maxSubArray(int[] nums) {
    int[] dp = new int[nums.length];
    dp[0] = 0;
    int max = 0;
    for (int i = 1; i < nums.length; i++) {
        dp[i] = nums[i] + (dp[i - 1] > 0 ? dp[i - 1] : 0);
        max = Math.max(dp[i], max);
    }
    return max;
}
```

### Q： 从 1 到 n 整数中 1 出现的次数

[解析](https://www.cnblogs.com/xuanxufeng/p/6854105.html)

```java
public int countDigitOne(int n) {
    int ones = 0;
    for (long long m = 1; m <= n; m *= 10) {
        int a = n / m, b = n % m;
        ones += (a + 8) / 10 * m + (a % 10 == 1) * (b + 1);
    }
    return ones;
}
```

### Q： 把数组排成最小的数

```java
public String PrintMinNumber(int [] numbers) {
    if (numbers.length == 0) {
        return "";
    }
    ArrayList<String> list = new ArrayList<String>();
    for (int i = 0; i < numbers.length; i++) {
        list.add(String.valueOf(numbers[i]));
    }
    Collections.sort(list, new Comparator<String>() {
        public int compare(String o1, String o2) {
            String tmp1 = o1 + o2;
            String tmp2 = o2 + o1;
            return tmp1.compareTo(tmp2);
        }
    });
    StringBuilder sb = new StringBuilder();
    for (String item : list) {
        sb.append(item);
    }
    return sb.toString();
}
```

### Q： 丑数

对于每一个丑数都要乘以 2,3,5 得到新的丑数，下一个丑数为新丑数中最小者，次小者需要保留下来，参与到下次计算丑数的过程中，所以每次只移动 t2,t3,t5三个指针中的一个

```java
public int GetUglyNumber_Solution(int index) {
    if (index < 7) return index;
    int[] uglys = new int[index];
    uglys[0] = 1;
    int t2 = 0, t3 = 0, t5 = 0, i;
    for (i = 1; i < index; i++) {
        uglys[i] = Math.min(uglys[t2] * 2, Math.min(uglys[t3] * 3, uglys[t5] * 5));
        if (uglys[i] == uglys[t2] * 2) t2++;
        if (uglys[i] == uglys[t3] * 3) t3++;
        if (uglys[i] == uglys[t5] * 5) t5++;
    }
    return uglys[uglys.length - 1];
}
```

### Q： 数组中的逆序对

思路：归并排序

```java
private int count = 0;
public int InversePairs(int[] array) {
    if (array == null || array.length == 0)
        return -1;
    mergeSort(array, 0, array.length - 1);
    return count;
}

public void mergeSort(int[] a, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(a, left, mid);  //左边归并排序，使得左子序列有序
        mergeSort(a, mid + 1, right); //右边归并排序，使得右子序列有序
        merge(a, left, mid, right); //将两个有序子序列合并
    }
}

public void merge(int[] a, int left, int mid, int right) {
    int[] tmp = new int[right - left + 1];
    int t = right - left;//临时数组下标
    int l = mid; //左子序列指针
    int r = right; //右子序列指针

    // 从右向左分别遍历子序列
    while (l >= left && r >= mid + 1) {
        if (a[l] > a[r]) {
            // 此时左右两子序列已各自有序，逆序对个数为 r - mid
            // 例如 57  46, 7与6构成逆序对，同时7与4也构成逆序对
            count += (r - mid);
            tmp[t--] = a[l--];
            if (count >= 1000000007) {
                count %= 1000000007;
            }
        } else {
            tmp[t--] = a[r--];
        }
    }
    while (l >= left) { // 将左子序列剩余元素填充进temp中
        tmp[t--] = a[l--];
    }
    while (r >= mid + 1) { // 将右子序列剩余元素填充进temp中
        tmp[t--] = a[r--];
    }
     //将temp中的元素全部拷贝到原数组中
    for (int i = 0; i <= right - left; i++) {
        a[left + i] = tmp[i];
    }
}
```

### Q： 数字在排序数组中出现的次数

```java
public int GetNumberOfK(int [] array , int k) {
    if (array.length == 0) {
        return 0;
    }
    int first = getFirst(array, k);
    int last = getLast(array, k);
    if (first != -1 && last != -1) {
        return last - first + 1;
    } else {
        return 0;
    }
}

public int getFirst(int[] array, int k) {
    int start = 0;
    int end = array.length - 1;
    while (start <= end) {
        int mid = (start + end) / 2;
        if (array[mid] == k) {
            if (mid == 0 || array[mid - 1] != k)
                return mid;
            else
                end = mid - 1;
        } else if (array[mid] > k) {
            end = mid - 1;
        } else {
            start = mid + 1;
        }
    }
    return -1;
}

public int getLast(int[] array, int k) {
    int start = 0;
    int end = array.length - 1;
    while (start <= end) {
        int mid = (start + end) / 2;
        if (array[mid] == k) {
            if (mid == array.length - 1 || array[mid + 1] != k)
                return mid;
            else
                start = mid + 1;
        } else if (array[mid] > k) {
            end = mid - 1;
        } else {
            start = mid + 1;
        }
    }
    return -1;
}
```

### Q： 二叉树的深度 & 判断是否为平衡二叉树

在这种方法中存在重复遍历二叉树节点的问题

```java
public boolean IsBalanced_Solution(TreeNode root) {
    if (root == null) {
        return true;
    }
    int left = TreeDepth(root.left);
    int right = TreeDepth(root.right);
    if (left - right > 1 || right - left > 1) {
        return false;
    }
    return IsBalanced_Solution(root.left) && IsBalanced_Solution(root.right);
}

// 计算二叉树的深度
public int TreeDepth(TreeNode root) {
    if (root == null) {
        return 0;
    }
    int left = TreeDepth(root.left);
    int right = TreeDepth(root.right);
    return Math.max(left, right) + 1;
}
```

上面的做法有很明显的问题，在判断上层结点的时候，会多次重复遍历下层结点，增加了不必要的开销。如果改为从下往上遍历，如果子树是平衡二叉树，则返回子树的高度；如果发现子树不是平衡二叉树，则直接停止遍历，这样至多只对每个结点访问一次。

```java
public boolean IsBalanced_Solution(TreeNode root) {
    return getDepth(root) != -1;
}

private int getDepth(TreeNode root) {
    if (root == null) return 0;
    int left = getDepth(root.left);
    if (left == -1) return -1;
    int right = getDepth(root.right);
    if (right == -1) return -1;
    return Math.abs(left - right) > 1 ? -1 : (1 + Math.max(left, right));
}
```

### Q： 数组中出现一次的数字

我们把数组中所有的数异或运算，最终得到的结果就是两个只出现一次的数字的异或结果。

因为两个只出现一次的数肯定不同，即他们的异或结果一定不为0，一定有一个位上有1。另外一个此位上没有1，我们可以根据此位上是否有1，将整个数组重新划分成两部分，一部分此位上一定有1，另一部分此位上一定没有1，然后分别对每部分求异或，因为划分后的两部分有这样的特点：其他数都出现两次，只有一个数只出现一次。因此，我们又可以运用异或运算，分别得到两部分只出现一次的数。

```java
public int[] singleNumber(int[] nums) {
    // Pass 1 :
    // Get the XOR of the two numbers we need to find
    int diff = 0;
    for (int num : nums) {
        diff ^= num;
    }
    // Get its last set bit
    diff &= -diff;

    // Pass 2 :
    int[] rets = {0, 0}; // this array stores the two numbers we will return
    for (int num : nums)
    {
        if ((num & diff) == 0) // the bit is not set
        {
            rets[0] ^= num;
        }
        else // the bit is set
        {
            rets[1] ^= num;
        }
    }
    return rets;
}
```

### Q： 和为S的两个数字 & 和为S的连续整数序列

和为S的两个数字：第一个指针指向数组第一个元素，第二个指针指向数组最有一个元素，当和等于S返回，和小于S移动第一个指针，和大于S移动第二个指针

```java
public ArrayList<Integer> FindNumbersWithSum(int [] array,int sum) {
    if (array.length == 0) {
        return new ArrayList<Integer>();
    }
    ArrayList<Integer> result = new ArrayList<Integer>();
    int first = 0;
    int last = array.length - 1;
    while (first < last) {
        int tmp = array[first] + array[last];
        if (tmp == sum) {
            result.add(array[first]);
            result.add(array[last]);
            break;
        } else if (tmp < sum) {
            first++;
        } else {
            last--;
        }
    }
    return result;
}
```

和为S的连续整数序列：初始化第一个指针为1，第二个指针为2，

- 和等于S时记录下来第一个指针到第二个指针之间的元素
- 和小于S时移动第二个指针，更新序列和
- 和大于S时移动第一个指针，更新序列和，如果向前移动第二指针会回到第二种情况，会造成死循环

由于序列时递增序列，所以在移动指针过程中要避免回头

```java
public ArrayList<ArrayList<Integer> > FindContinuousSequence(int sum) {
    if (sum < 3) {
        return new ArrayList<ArrayList<Integer>>();
    }
    ArrayList<ArrayList<Integer>> result = new ArrayList<ArrayList<Integer>>();
    int first = 1;
    int last = 2;
    int mid = (sum + 1) / 2;
    int curSum = first + last;
    while (first < mid) {
        if (curSum == sum) {
            ArrayList<Integer> list = new ArrayList<Integer>();
            for (int i = first; i <= last; i++) {
                list.add(i);
            }
            result.add(list);
            curSum -= first;
            first++;
        } else if (curSum < sum) {
            last++;
            curSum += last;
        } else {
            curSum -= first;
            first++;
        }
    }
    return result;
}
```

### Q： 反转单词顺序 & 左旋转字符串

反转单词顺序：`I am a student` -> `student a am I`，先反转整个字符串，然后反转每个单词

```java
public String ReverseSentence(String str) {
    if (str.isEmpty()) {
        return "";
    }
    char[] cs = str.toCharArray();
    reverse(cs, 0, cs.length - 1);
    int start = 0, end = 0;
    while (end != cs.length) {
        if (end == cs.length - 1 || cs[end + 1] == ' ') {
            reverse(cs, start, end);
            start = end + 2;
        }
        end++;
    }
    return String.valueOf(cs);
}
public void reverse(char[] cs, int start, int end) {
    while (start < end) {
        char tmp = cs[start];
        cs[start] = cs[end];
        cs[end] = tmp;
        start++;
        end--;
    }
}
```

左旋转字符串

左旋转即向左移动字符，先旋转前n个字符，再旋转后面的字符，最后整体旋转
右旋转即向右移动字符，先整体旋转，再旋转前n个字符，最后旋转后面的字符

```java
public String LeftRotateString(String str,int n) {
    if (str.isEmpty()) return "";
    char[] cs = str.toCharArray();
    reverse(cs, 0, n - 1);
    reverse(cs, n, cs.length - 1);
    reverse(cs, 0, cs.length - 1);
    return String.valueOf(cs);
}
public void reverse(char[] cs, int start, int end) {
    while (start < end) {
        char tmp = cs[start];
        cs[start] = cs[end];
        cs[end] = tmp;
        start++;
        end--;
    }
}
```

### Q： 扑克牌顺子

- 序列中存在两个相同的数字即不是顺子
- 序列中不连续的位数要小于大小王的个数

```java
public boolean isContinuous(int [] numbers) {
    if (numbers.length == 0) {
        return false;
    }
    Arrays.sort(numbers);
    int numZero = 0;
    for (int i = 0; i < numbers.length; i++) {
        if (numbers[i] == 0) {
            numZero++;
        }
    }
    int numGap = 0;
    int start = numZero; // 从第一个非0数字后开始遍历数组
    int end = start + 1;
    while (end < numbers.length) {
        if (numbers[start] == numbers[end]) {
            return false;
        }
        numGap += (numbers[end] - numbers[start] - 1);
        start = end;
        end++;
    }
    return (numGap > numZero) ? false : true;
}
```

### Q： 求 1 + 2 +3 + ... + n

利用构造函数求解

```java

```