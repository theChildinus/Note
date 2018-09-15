# LeetCode 笔记

## **Array**

### 题目：从数组中删除元素

题号：27，26，80

基本思路:

- 尝试使用两个指针
- 时间复杂度：O(n)，空间复杂度：O(1)

```java
// 数组中删除指定元素
public int removeElement(int[] nums, int val) {
    int i = 0;
    for (int j = 0; j < nums.length; j++) {
        if (nums[j] != val) {
            nums[i] = nums[j];
        }
    }
    return i;
}
```

```java
// 排序好的数组，每个数字只允许出现一次
public int removeDuplicates(int[] nums) {
    if (nums.length == 0) {
        return 0;
    }

    int n = 1;
    for (int i  = 1; i < nums.length; i++) {
        if (nums[i] != nums[i - 1]) {
            nums[n] = nums[i];
            n++;
        }
    }
    return n;
}
```

```java
// 排序好的数组，每个数字最多出现两次
// 自己的解法
public int removeDuplicates(int[] nums) {
    if (nums.length == 0) {
        return 0;
    }

    int n = 1;
    int count = 1;
    for (int i = 1; i < nums.length; i++) {
        if (nums[i] != nums[i - 1]) {
            nums[n] = nums[i];
            count = 1;
            n++;
        }
        else if (nums[i] == nums[i - 1] && count < 2) {
            nums[n] = nums[i];
            count++;
            n++;
        }
    }
    return n;
}

// LeetCode上的解法
public int removeDuplicates(int[] nums) {
    int i = 0;
    for (int n : nums)
        if (i < 2 || n > nums[i-2])
            nums[i++] = n;
    return i;
}
```

### 题目：旋转数组

题号：189

```java
// 基本旋转
public void reverse(int[] nums, int begin, int end) {
    while (start < end) {
        int tmp = nums[start];
        nums[start] = nums[end];
        nums[end] = tmp;
        start++;
        end--;
    }
}
```

```java
// 右旋，向右移动
public void rotate(int[] nums, int k) {
    k %= nums.length;
    reverse(nums, 0, nums.length - 1);
    reverse(nums, 0, k - 1);
    reverse(nums, k, nums.lenght - 1);
}
```

```java
// 左旋，向左移动
public void rotate(int[] nums, int k) {
    k %= nums.length;
    reverse(nums, 0, k - 1);
    reverse(nums, k, nums.lenght - 1);
    reverse(nums, 0, nums.length - 1);
}
```

### 题目：从未排序的整数数组中找出最小的未出现的正整数

题号：41

思路：将每个数字放到正确的位置上，然后从前往后遍历，找到第一个数组元素与下标不相等的地方，返回 place ＋ 1

```java
    public int firstMissingPositive(int[] nums) {
        for (int i = 0; i < nums.length; i++) {
            while (nums[i] > 0 && nums[i] <= nums.length && nums[i] != nums[nums[i] - 1]) {
                int pos = nums[i] - 1;
                int tmp = nums[i];
                nums[i] = nums[pos];
                nums[pos] = tmp;
            }
        }

        for (int i = 0; i < nums.length; i++) {
            if (nums[i] > 0 && nums[i] != i + 1) {
                return i + 1;
            }
        }
        return nums.length + 1;
    }

```

### 问题：公牛母牛游戏

题号：299

问题描述：给定一个数字，别人来猜，猜的数字和给定的数字进行比较，位数相同且对应位置数字也相同的，被称作bulls；数字相同但位数不同的叫做cows。要求就是给出bulls和cows的个数，方法结构如下，数字都以字符串的形式表现，返回值也是，其中，A表示bulls，B表示cows。

这里需要注意，如果bulls中已经出现的数字，就不能在出现在cows中了

思路1：总匹配次数减去bulls匹配次数

```java
public String getHint(String secret, String guess) {
    char[] secretTable = new char[10];
    char[] guessTable = new char[10];

    for (int i = 0; i < secret.length(); i++) {
        secretTable[secret.charAt(i) - '0']++;
        guessTable[guess.charAt(i) - '0']++;
    }

    int matchs = 0;
    for (int i = 0; i < secretTable.length; i++) {
        matchs += Math.min(secretTable[i], guessTable[i]);
    }

    int bulls = 0;
    for (int i = 0; i < secret.length(); i++) {
        if (secret.charAt(i) == guess.charAt(i)) {
            bulls++;
        }
    }
    int cows = matchs - bulls;
    return bulls + "A" + cows + "B";
}
```

思路2：用hash表统计不同位字符出现频率，然后用guess的字符串来匹配 (速度较慢)

```java
public String getHint(String secret, String guess) {
    Map<Character, Integer> count = new HashMap<Character, Integer>();
    int bulls = 0, matches = 0;
    for (int i = 0; i < secret.length(); i++) {
        char s = secret.charAt(i);
        if (secret.charAt(i) == guess.charAt(i)) bulls++;
        if (!count.containsKey(s)) {
            count.put(s, 1);
        }
        else {
            int newValue = count.get(s) + 1;
            count.put(s, newValue);
        }
    }

    for (int i = 0; i < guess.length(); i++) {
        char g = guess.charAt(i);
        if (count.containsKey(g) && count.get(g) > 0) {
            matches++;
            int newValue = count.get(g) - 1;
            count.put(g, newValue);
        }
    }
    return bulls + "A" + (matches - bulls) + "B";
}
```

### 问题：Gas Station

题号：134

思路：累加在每个位置的 tank += gas[i] - cost[i], 就是在每个位置剩余的油量, 如果left一直大于0, 就可以一直走下取. 如果left小于0了, 那么就从下一个位置重新开始计数, 并且将之前欠下的多少记录下来(total), 如果最终遍历完数组剩下的燃料足以弥补之前不够的, 那么就可以到达, 并返回最后一次开始的位置.否则就返回-1.

证明：

- 1.如果从头开始, 每次累计剩下的油量都为整数, 那么没有问题, 他可以从头开到结束.
- 2.如果到中间的某个位置, 剩余的油量为负了, 那么说明之前累积剩下的油量不够从这一站到下一站了. 那么就从下一站从新开始计数. 为什么是下一站, 而不是之前的某站呢? 因为第一站剩余的油量肯定是大于等于0的, 然而到当前一站油量变负了, 说明从第一站之后开始的话到当前油量只会更少而不会增加. 也就是说从第一站之后, 当前站之前的某站出发到当前站剩余的油量是不可能大于0的. 所以只能从下一站重新出发开始计算从下一站开始剩余的油量, 并且把之前欠下的油量也累加起来, 看到最后剩余的油量是不是大于欠下的油量.

```java
public int canCompleteCircuit(int[] gas, int[] cost) {
    int start = 0, total = 0, tank = 0;
    for (int i = 0; i < gas.length; i++) {
        tank += gas[i] - cost[i]; // 累加
        if (tank < 0) { // 到某一点不可达
            start = i + 1;
            total += tank;
            tank = 0;
        }
    }
    return (total + tank < 0) ? -1 : start;
}
```

### 问题：众数

题号：169，229

该数字出现次数大于 n/2

思路：投票算法，因为符合的众数总是存在。所以每出现一对不一样的就可以忽视这一对。我们可以用计数表示，来一个数字，如果和candidate相同那么count++，否则count--，如果count为零了，那么candidate就是下一个数。最后candidate就是众数了

```java
public int majorityElement(int[] nums) {
    int count = 0;
    Integer candidate = null;
    for (int num : nums) {
        if (count == 0) {
            candidate = num;
        }
        count += (num == candidate) ? 1 : -1;
    }
    return candidate;
}
```

该数字出现次数大于 n/3

思路：三三抵消，最后会剩下两个candidates，但是注意此时不是谁占多数谁是最终结果，反例[1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 4, 4] 三三抵消后剩下 [1, 4，4] 4数量占优，但结果应该是1，所以三三抵消后，再loop一遍找1和4谁数量超过了len(nums)/3，另外一个反例[3,2,3]，2是candidate但不是结果

```java
public List<Integer> majorityElement(int[] nums) {
    if (nums == null || nums.length == 0)
        return new ArrayList<Integer>();
    List<Integer> result = new ArrayList<Integer>();
    int number1 = nums[0], number2 = nums[0], count1 = 0, count2 = 0, len = nums.length;
    for (int i = 0; i < len; i++) {
        if (nums[i] == number1)
            count1++;
        else if (nums[i] == number2)
            count2++;
        else if (count1 == 0) {
            number1 = nums[i];
            count1 = 1;
        } else if (count2 == 0) {
            number2 = nums[i];
            count2 = 1;
        } else {
            count1--;
            count2--;
        }
    }
    count1 = 0;
    count2 = 0;
    for (int i = 0; i < len; i++) {
        if (nums[i] == number1)
            count1++;
        else if (nums[i] == number2)
            count2++;
    }
    if (count1 > len / 3)
        result.add(number1);
    if (count2 > len / 3)
        result.add(number2);
    return result;
}
```

### 问题：hIndex 影响因子

题号：274，275

描述：h 因子兼顾研究学术人员的学术产出数量与学术产出质量。假设一个研究者的 h 因子为 10，则表明该研究者被引用次数大于等于 10 的文章数量也应大于等于 10

实例 citations = [3, 0, 6, 1, 5]，为什么是3呢？因为引用次数大于等于3的文章为citations = [**3**, 0, **6**, 1, **5**]，加粗部分，显然判定该学者 h 因子为6或者5是不合适的

思路1：先将数组排序。然后对于每个引用次数，比较大于该引用次数的文章，取引用次数和文章数的最小值，即 Math.min(citations.length-i, citations[i])，并更新 level，取最大值。

```java
public int hIndex(int[] citations) {
    Arrays.sort(citations);
    int level = 0;
    for(int i = 0; i < citations.length; i++)
        level = Math.max(level,Math.min(citations.length - i,citations[i]));
    return level;
}
```

思路2：借用 hash 表降低复杂度，我们使用一个大小为 n+1 的数组 count，对于 count[i]表示的是引用数为 i 的文章数量。从后往前遍历数组，当满足 count[i] >= i 时，i 就是 h 因子，返回即可，否则返回0

因为引用数大于i-1的数量是i-1及之后的累加，必须从后往前遍历

```java
public int hIndex(int[] citations) {
    int n = citations.length;
    int[] count = new int[n + 1];
    for(int c : citations)
        if(c >= n) count[n]++;  //当引用数大于等于 n 时，我们均将其数量计入 count[n]中
        else count[c]++;
    for(int i = n; i > 0; i--) {  //从后面开始遍历
        if(count[i] >= i) return i;
        count[i-1] += count[i];  //引用数大于 i-1 的数量是i-1及之后的累加
    }
    return 0;
}
```

对于排序好的数组，进行二分查找

```java
public int hIndex(int[] citations) {
    int n = citations.length;
    int low = 0;
    int high = n - 1;
    int ans = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (citations[mid] >= n - mid) { // 发现引用次数大于n-mid 就在左半部分查找可能更大的level
            ans = n - mid;
            high = mid - 1;
        } else { // 说明左半部分没有更大的level 在右半部分查找
            low = mid + 1;
        }
    }
    return ans;
}
```

### 问题：Contains Duplicate

题号：217，219，220

217：给定的数组，看是否有重复元素

方法：暴力，排序，set

219：给定一个整数数组和一个整数 k，判断数组中是否存在两个不同的索引 i 和 j，使得 nums [i] = nums [j]，并且 i 和 j 的差的绝对值最大为 k

思路：建立一个长度为k的set，用它来扫描整个数组，不断地判断新出现的数据能不能放进去，如果不能放进去，说明存在距离小于等于k的数据是有相等的，否则就可以放进去，当然set中的数据量如果超过k了就要同时把早先放进去的数据拿出来了

```java
public boolean containsNearbyDuplicate(int[] nums, int k) {
    Set<Integer> set = new HashSet<Integer>();
    for(int i = 0; i < nums.length; i++){
        if(i > k) set.remove(nums[i-k-1]);
        if(!set.add(nums[i])) return true;
    }
    return false;
 }
```

220：给定一个整数数组，判断数组中是否有两个不同的索引 i 和 j，使得 nums [i] 和 nums [j] 的差的绝对值最大为 t，并且 i 和 j 之间的差的绝对值最大为 k

思路:平衡树的方法 复杂度 nlogk

题意有：`-t <= x - nums[i] <= t` 左边有 `nums[i] - t <= x` 因此把符合条件的数构建成一颗平衡树，然后查找一个最小的x使得 `x >= nums[i] - t`

或者找到 x 满足 `x  <= t + nums[i]`

```java
public boolean containsNearbyAlmostDuplicate(int[] nums, int k, int t) {
    if (nums == null || nums.length < 2 || k < 1) return false;

    TreeSet<Long> set = new TreeSet<>();
    for (int i = 0; i < nums.length; i++) {
        long l = (long)nums[i];

        Long floor = set.floor(l); // set中小于等于给定元素 的最大元素
        Long ceil = set.ceiling(l); // set中大于等于给定元素 的最小元素
        if (floor != null && l - floor <= t ||
                ceil != null && ceil - l <= t) {
            return true;
        }

        set.add(l);
        if (i >= k) { // 维护窗口大小为k
            set.remove((long)nums[i - k]);
        }
    }
    return false;
}
```

### 问题：Jump Game

题号：55, 45

描述：

- Given an array of non-negative integers, you are initially positioned at the first index of the array.
- Each element in the array represents your maximum jump length at that position.
- Determine if you are able to reach the last index.

beats 100% :satisfied:

贪心算法（又称贪婪算法）是指，在对问题求解时，总是做出在当前看来最好的选择。也就是说，不从整体最优上加以考虑，他所作出的是在某种意义上的局部最优解。贪心算法和动态规划算法都是由局部最优导出全局最优，这里不得不比较下二者的区别

贪心算法：

- 1.贪心算法中，作出的每步贪心决策都无法改变，因为贪心策略是由上一步的最优解推导下一步的最优解，**而上一步之前的最优解则不作保留。**
- 2.由1中的介绍，可以知道贪心法正确的条件是：每一步的最优解一定包含上一步的最优解

动态规划算法：

- 1.全局最优解中一定包含某个局部最优解，但不一定包含前一个局部最优解，**因此需要记录之前的所有最优解**
- 2.动态规划的关键是状态转移方程，即如何由以求出的局部最优解来推导全局最优解
- 3.边界条件：即最简单的，可以直接得出的局部最优解

[Solution](https://leetcode.com/problems/jump-game/solution/)

```java
public boolean canJump(int[] nums) {
    int n = nums.length;
    if (n == 1) return true;
    int i = n - 2;
    int j = n - 1;
    while (i >= 0) {
        if (i + nums[i] >= j) {
            j = i;
        }
        i--;
    }

    return j == 0;
}
```

45题 要求计算出最少的跳转次数

sc: current step. max: current furthest. e: current furthest with one more step

从前向后遍历，只在`i == e`，即当前位置i达到当前最远距离时，步数加1，此时为 `<i` 范围内的最优解

```java
public int jump(int[] nums) {
    int sc = 0;
    int e = 0;
    int max = 0;
    for (int i = 0; i < nums.length - 1; i++) {
        max = Math.max(max, i + nums[i]);
        if (i == e) {
            sc++;
            e = max;
        }
    }
    return sc;
}
```

### 问题：Best Time to Buy and Sell Stock I II III IV

[参考：状态机解法](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/discuss/149383/Easy-DP-solution-using-state-machine-O(n)-time-complexity-O(1)-space-complexity)

!!!在每次循环内，要更新所有状态机中的状态，可以在某一天卖出之后，买入再卖出

题号：121，122，123，188，309

121:
思路：只需要遍历一遍数组，用一个变量记录遍历过数中的最小值，然后每次计算当前值和这个最小值之间的差值最为利润，然后每次选较大的利润来更新。当遍历完成后当前利润即为所求

```java
public class Solution {
    public int maxProfit(int prices[]) {
        int minprice = Integer.MAX_VALUE;
        int maxprofit = 0;
        for (int i = 0; i < prices.length; i++) {
            if (prices[i] < minprice)
                minprice = prices[i];
            else if (prices[i] - minprice > maxprofit)
                maxprofit = prices[i] - minprice;
        }
        return maxprofit;
    }
}
```

122: 不限制交易次数

如果数组是单调不减的，例如[1, 4, 5, 9]，(4 - 1) + (5 - 4) + (9 - 5) = (9 - 1)

如果数组中存在逆向序列，例如[1, 5, 3, 6]，最大利润只可能是(5 - 1) + (6 - 3)，(6 - 1)必然小于该利润

不存在首尾差值大于子序列差值和的情况

```java
public int maxProfit(int[] prices) {
    int maxProfit = 0;
    for (int i = 1; i < prices.length; i++) {
        if (prices[i] > prices[i - 1]) {
            maxProfit += prices[i] - prices[i - 1];
        }
    }
    return maxProfit;
}
```

123: 最多允许两笔交易

思路：在每一天，我们以尽可能低的价格购买股票，并以尽可能高的价格出售股票。对于第二笔交易，我们将第一笔交易的利润整合到第二笔交易的成本中，然后第二笔交易的利润将是两笔交易的总利润

```java
public int maxProfit(int[] prices) {
    int n = prices.length;
    if (n <= 1) return 0;
    // 相当于第一题加强版

    int buy1 = Integer.MIN_VALUE, sell1 = 0;
    int buy2 = Integer.MIN_VALUE, sell2 = 0;
    for (int price : prices) {
        // 第一次买入，始终买当前所有里面最便宜的那一次，第一次买，利润肯定为负，但是要找出最少的那一个
        if (buy1 < -price) buy1 = -price;
        // 第一次卖出 可能的最大利润
        if (sell1 < buy1 + price) sell1 = buy1 + price;
        // 第二次的利润已经包含第一次的利润，第二次买入
        if (buy2 < sell1 - price) buy2 = sell1 - price;
        // 最后一次卖出
        if (sell2 < buy2 + price) sell2 = buy2 + price;
    }
    return sell2;
}
```

188：最多允许k次交易

```java
public int maxProfit(int k, int[] prices) {
    if (k <= 0 || prices.length <= 1) return 0;
    if (k > prices.length / 2) k = prices.length + 1;

    int[] stock = new int[k * 2];
    Arrays.fill(stock, Integer.MIN_VALUE);
    stock[0] = -prices[0];

    for (int i = 1; i < prices.length; i++) {
        stock[0] = Math.max(stock[0], -prices[i]);
        for (int j = 1; j < k * 2; j += 2) {
            stock[j] = Math.max(stock[j], stock[j - 1] + prices[i]); // selling
            if (j + 1 < k * 2) {
                stock[j + 1] = Math.max(stock[j + 1], stock[j] - prices[i]);  // buying
            }
        }
    }
    return Math.max(stock[k * 2 - 1], 0);
}
```

309：可以进行无数次交易但是刚交易完不允许在下一天交易

思路：

- 在第i天买一支股票还能剩下的利润 ＝ 第(i-2)天销售能够剩余的利润 - 第i天股票的价钱
- 在第i天卖一支股票总的利润 ＝ 第(i-1)天买股票剩下的最大利润＋当前股票的价格

```java
class Solution {
    public int maxProfit(int[] prices) {
        int n = prices.length;
        if (n <= 1) return 0;
        int[] buy = new int[n];
        int[] sell = new int[n];
        buy[0] = -prices[0];
        buy[1] = Math.max(-prices[0], -prices[1]);
        sell[1] = Math.max(0, prices[1] - prices[0]);
        for (int i = 2; i < n; i++){
            buy[i] = Math.max(buy[i - 1], sell[i - 2] - prices[i]);
            sell[i] = Math.max(sell[i - 1], buy[i - 1] + prices[i]);
        }
        return sell[n - 1];
    }
}
```

### 问题：Container With Most Water

题号：11

[思路](https://leetcode.com/problems/container-with-most-water/discuss/6100/Simple-and-clear-proofexplanation)

1. 所有其他容器的宽度较小，因此需要更高的水位才能容纳更多的水。
2. 第一行和最后一行中较小的一行不支持较高的水位，因此可以安全地从进一步的考虑中删除。

```java
public int maxArea(int[] height) {
    int i = 0, j = height.length - 1;
    int maxArea = 0;
    while (i < j) {
        maxArea = Math.max(maxArea, Math.min(height[i], height[j]) * (j - i));
        if (height[i] < height[j]) {
            i++;
        } else {
            j--;
        }
    }
    return maxArea;
}
```

### 问题：Trapping Rain Water

题号：42

问题描述：雨水积累

思路：两高夹一矮才可能存到水

- 从左向右遍历数组，找到每个元素左侧最大的数
- 从右向左遍历数组，找到每个元素右侧最大的数
- 某处的积水量 = min(left[i], right[i]) - height[i]

```java
public int trap(int[] height) {
    int n = height.length;
    if (n <= 2) return 0;
    int[] left = new int[height.length];
    int[] right = new int[height.length];

    int lmax = height[0];
    int rmax = height[n - 1];
    for (int i = 0; i < n; i++) {
        if (height[i] > lmax) {
            left[i] = height[i];
            lmax = height[i];
        } else {
            left[i] = lmax;
        }

    }
    for (int j = n - 1; j >= 0; j--) {
        if (height[j] > rmax) {
            right[j] = height[j];
            rmax = height[j];
        } else {
            right[j] = rmax;
        }
    }

    int ret = 0;
    for (int i = 0; i < n; i++) {
        ret += Math.min(left[i], right[i]) - height[i];
    }
    return ret;
}
```

优化：将left数组和right数组改为left_max和right_max两个值

```java
int trap(vector<int>& height)
{
    int left = 0, right = height.size() - 1;
    int ans = 0;
    int left_max = 0, right_max = 0;
    while (left < right) {
        if (height[left] < height[right]) {
            height[left] >= left_max ? (left_max = height[left]) : ans += (left_max - height[left]);
            ++left;
        }
        else {
            height[right] >= right_max ? (right_max = height[right]) : ans += (right_max - height[right]);
            --right;
        }
    }
    return ans;
}
```

### 问题：求数组中是否包含三个元素递增的子序列

题号：334

思路：记录最小和第二小的元素位置，找到一个比两个都大的元素返回true

```java
    public boolean increasingTriplet(int[] nums) {
        if(nums.length < 3)
            return false;

        int small = Integer.MAX_VALUE, big = Integer.MAX_VALUE;
        for (int n : nums) {
            if (n <= small) { small = n; } // update small if n is smaller than both
            else if (n <= big) { big = n; } // update big only if greater than small but smaller than big
            else return true; // return if you find a number bigger than both
        }
        return false;
    }
```

### 问题：最长的连续序列

题号：128

思路1:排序

```java
public int longestConsecutive(int[] nums) {
    if (nums.length == 0) {
        return 0;
    }

    Arrays.sort(nums);

    int longestStreak = 1;
    int currentStreak = 1;

    for (int i = 1; i < nums.length; i++) {
        if (nums[i] != nums[i-1]) {
            if (nums[i] == nums[i-1]+1) {
                currentStreak += 1;
            }
            else {
                longestStreak = Math.max(longestStreak, currentStreak);
                currentStreak = 1;
            }
        }
    }

    return Math.max(longestStreak, currentStreak);
}

```

思路2:hashset

```java
public int longestConsecutive(int[] nums) {
    Set<Integer> num_set = new HashSet<Integer>();
    for (int num : nums) {
        num_set.add(num);
    }

    int longestStreak = 0;

    for (int num : num_set) {
        if (!num_set.contains(num-1)) { // num为左边界元素，即子序列中起始的元素
            int currentNum = num;
            int currentStreak = 1;
            // 查找自序列的右边界，记录长度
            while (num_set.contains(currentNum+1)) {
                currentNum += 1;
                currentStreak += 1;
            }

            longestStreak = Math.max(longestStreak, currentStreak);
        }
    }

    return longestStreak;
}
```

### 查找重复元素

题号：287

思路：sort，set，[Floyd's Tortoise and Hare](https://blog.csdn.net/wr339988/article/details/53617914)

[1, 2, 4, 3, 2]中，前一个2是A点，代表重复的数，从该位置进入环，后一个2是两个指针相遇的点，慢指针是一步一步走，快指针是两步两步走

```java
public int findDuplicate(int[] nums) {
    // Find the intersection point of the two runners.
    int tortoise = nums[0];
    int hare = nums[0];
    do {
        tortoise = nums[tortoise];
        hare = nums[nums[hare]];
    } while (tortoise != hare);

    // Find the "entrance" to the cycle.
    int ptr1 = nums[0];
    int ptr2 = tortoise;
    while (ptr1 != ptr2) {
        ptr1 = nums[ptr1];
        ptr2 = nums[ptr2];
    }

    return ptr1;
}
```

## **String**

### 问题：Longest Common Prefix

题号：14

思路：用第一字符作为母串，后面的字符串挨个去匹配，查找最长前缀

```java
public String longestCommonPrefix(String[] strs) {

    String prefix = "";
    String temp = "";

    if (strs.length == 0) return prefix;
    if (strs.length == 1) return strs[0];
    for (int i = 1; i <= strs[0].length(); i++) {
        prefix = temp;
        temp = strs[0].substring(0,i);
        for (int j = 1; j < strs.length; j++) {
            if (strs[j].indexOf(temp)!=0) return prefix;
        }
    }

    return temp;
}
```

### 问题：Length of Last Word

题号：58

思路：从后向前遍历字符串，记录最后一个单词长度，遇到空格返回

```java
public int lengthOfLastWord(String s) {
    if (s == null || s.length() == 0) return 0;
    int n = s.length();
    int cnt = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (s.charAt(i) != ' ') cnt++;
        else {
            if (cnt > 0) return cnt;
        }
    }
    return cnt;
}
```

### 问题：寻找第一个只出现一次的字符串

题号：387

思路：统计法，另外一种，分别从前往后 和 从后往前 找字符下标，两个下标相同即该字符出现了一次的

```java
public int firstUniqChar(String s) {
    if (s == null || s.length() == 0) return -1;

    int res = s.length();
    for (int i = 'a'; i <= 'z'; i++) {
        int index = s.indexOf(i);
        if (index == -1) continue;
        if (index == s.lastIndexOf(i))
            res = Math.min(res, index);
    }

    return res == s.length() ? - 1 : res;
}
```

### 问题：同构字符串

题号：205 290

```txt
Input: s = "egg", t = "add"
Output: true
```

```txt
Input: s = "foo", t = "bar"
Output: false
```

思路：建立从s串到t串的映射，不允许不同字符映射到同一个字符上，将s串用t串替换，最后比较s串和t串

```java
public boolean isIsomorphic(String s, String t) {
    if (s == null || t == null) return false;

    StringBuilder sb = new StringBuilder(s);
    Map<Character, Character> map = new HashMap<>();

    for(int i = 0; i < s.length(); i++) {
        char ss = sb.charAt(i);
        char tt = t.charAt(i);
        if (!map.containsKey(ss)) {
            if (!map.containsValue(tt)) {
                map.put(ss, tt);
                sb.setCharAt(i, tt);
            }
        } else {
            sb.setCharAt(i, map.get(ss));
        }
    }

    if (sb.toString().equals(t)) return true;
    else return false;
}
```

### 问题：聚集同类型的字符串

题号：49

```txt
Input: ["eat", "tea", "tan", "ate", "nat", "bat"],
Output:
[
  ["ate","eat","tea"],
  ["nat","tan"],
  ["bat"]
]
```

思路：利用排序，对每个字符串排序在map中找到相同key的，然后插入

```java
public List<List<String>> groupAnagrams(String[] strs) {
    if (strs == null || strs.length == 0) return new ArrayList<List<String>>();

    Map<String, List<String>> map = new HashMap<String, List<String>>();
    for (String s : strs) {
        // 排序的地方可以优化，用计数来代替 例如abc = #1#1#1#0... abb = #1#2#0#0...
        char[] ca = s.toCharArray();
        Arrays.sort(ca);
        String keyStr = String.valueOf(ca);
        if (!map.containsKey(keyStr)) map.put(keyStr, new ArrayList<String>());
        map.get(keyStr).add(s);
    }
    return new ArrayList<List<String>>(map.values());
}
```

### 问题：最大的数

题号：179

思路：取两个数，取两种组合中较大的一种进行排序

```java
class Solution {
    private class LargerNumberComparator implements Comparator<String> {
        @Override
        public int compare(String a, String b) {
            String order1 = a + b;
            String order2 = b + a;
           return order2.compareTo(order1);
        }
    }

    public String largestNumber(int[] nums) {
        // Get input integers as strings.
        String[] asStrs = new String[nums.length];
        for (int i = 0; i < nums.length; i++) {
            asStrs[i] = String.valueOf(nums[i]);
        }

        // Sort strings according to custom comparator.
        Arrays.sort(asStrs, new LargerNumberComparator());

        // If, after being sorted, the largest number is `0`, the entire number
        // is zero.
        if (asStrs[0].equals("0")) {
            return "0";
        }

        // Build largest number from sorted array.
        String largestNumberStr = new String();
        for (String numAsStr : asStrs) {
            largestNumberStr += numAsStr;
        }

        return largestNumberStr;
    }
}
```

### 问题：Remove Duplicate Letters

题号：316

```txt
Input: "bcabc"
Output: "abc"
```

```txt
Input: "cbacdcbc"
Output: "acdb"
```

思路：[连接](https://leetcode.com/problems/remove-duplicate-letters/discuss/76762/Java-O(n)-solution-using-stack-with-detail-explanation)



```java
public String removeDuplicateLetters(String s) {
    Stack<Character> stack = new Stack<>();
    int[] count = new int[26];
    char[] arr = s.toCharArray();
    for(char c : arr) {
        count[c-'a']++;
    }
    boolean[] visited = new boolean[26];
    for(char c : arr) {
        count[c-'a']--;
        if(visited[c-'a']) {
            continue;
        }
        // 不断更新堆栈，使堆栈中的元素尽可能的小
        // count[stack.peek() - 'a'] > 0 用于判断后面的字符中是否还存在该字符，如果存在就可以从栈中弹出，如果不存在就不能弹出
        while(!stack.isEmpty() && stack.peek() > c && count[stack.peek()-'a'] > 0) {
            visited[stack.peek()-'a'] = false;
            stack.pop();
        }
        stack.push(c);
        visited[c-'a'] = true;
    }
    StringBuilder sb = new StringBuilder();
    for(char c : stack) {
        sb.append(c);
    }
    return sb.toString();
}
```

### 问题：Excel Sheet Column Title

题号：168

```txt
    1 -> A
    2 -> B
    3 -> C
    ...
    26 -> Z
    27 -> AA
    28 -> AB
    ...
```

```java
public String convertToTitle(int n) {
    if (n == 0) return "";

    StringBuilder sb = new StringBuilder();
    while (n > 0) {
        n--;
        sb.insert(0, (char)('A' + n % 26));
        n /= 26;
    }
    return sb.toString();
}
```

反过来：

```java
public int titleToNumber(String s) {
    if (s == null) return -1;
    int ans = 0;
    int power = 1;
    for (char c : s.toCharArray()) {
        ans *= 26;
        ans += c - 'A' + 1;
    }
    return ans;
}
```

### 问题：Longest Palindromic Substring 最长回文

题号：5

思路：动态规划

假设`dp[i][j]`表示字串`s[i...j]`是否是回文，那么对于动态规划dp的表达方式为：

初始化：

- `dp[i][i] = true` `0 <= i <= n - 1`
- `dp[i][i + 1] = true`
  - `if s[i] == s[i + 1]`
- `others = false`

动态规划状态转移方程

- `dp[i][j] = true`
  - `dp[i + 1][j - 1] == true` && `if s[i] == s[j]`
- `dp[i][j] = false`
  - `if s[i] != s[j]`

```java
public String longestPalindrome(String s) {
    int n = s.length();
    String res = "";

    boolean[][] dp = new boolean[n][n];

    for (int i = n - 1; i >= 0; i--) {
        for (int j = i; j < n; j++) {
            dp[i][j] = s.charAt(i) == s.charAt(j) && (j - i < 3 || dp[i + 1][j - 1]);

            if (dp[i][j] && (res == null || j - i + 1 > res.length())) {
                res = s.substring(i, j + 1);
            }
        }
    }

    return res;
}
```

思路：中心法，从中心向两边扩充，分为两种情况，aba从b开始向两边扩充，abba从中间的bb向两边扩充

```java
private int expandAround(String s, int left, int right) {
    int l = left, r = right;
    while (l >= 0 && r < s.length() && s.charAt(l) == s.charAt(r)) {
        l--;
        r++;
    }
    return r - l - 1;
}

public String longestPalindrome(String s) {
    if (s.isEmpty()) return "";
    if (s.length() == 1) return s;

    int start = 0, end = 0;
    for (int i = 0; i < s.length(); i++) {
        int len1 = expandAround(s, i, i);
        int len2 = expandAround(s, i, i + 1);
        int len = Math.max(len1, len2);
        if (len > end - start) {
            start = i - (len - 1) / 2;
            end = i + len / 2;
        }
    }
    return s.substring(start, end + 1);
}
```

### 问题：Generate Parentheses

题号：22

思路：递归

```java
public List<String> generateParenthesis(int n) {
        List<String> list = new ArrayList<String>();
        backtrack(list, "", 0, 0, n);
        return list;
    }

    public void backtrack(List<String> list, String str, int open, int close, int max){

        if(str.length() == max*2){
            list.add(str);
            return;
        }

        // 左括号少于指定个数n时添加
        if(open < max)
            backtrack(list, str+"(", open+1, close, max);
        // 右括号少于左括号的时候才能添加
        if(close < open)
            backtrack(list, str+")", open, close+1, max);
}
```

## Math

### 问题：Plus One

题号：66

```txt
Input: [1,2,3]
Output: [1,2,4]
Explanation: The array represents the integer 123.
```

```java
public int[] plusOne(int[] digits) {
    if(digits == null || digits.length == 0) {
        return new int[]{};
    }

    List<Integer> result = new ArrayList<>();
    int sum = 1;
    for(int i = digits.length - 1; i >= 0; i--) {
        sum += digits[i];
        result.add(0, sum % 10);
        sum /= 10;
    }
    if(sum != 0) {
        result.add(0, sum);
    }
    int[] number = new int[result.size()];
    for(int i = 0; i < number.length; i++) {
        number[i] = result.get(i);
    }
    return number;
}
```

### 问题：Add Binary

题号：67

```java
public String addBinary(String a, String b) {
    StringBuilder sb = new StringBuilder();
    int c = 0;
    int i = a.length() - 1;
    int j = b.length() - 1;
    while (i >= 0 && j >= 0) {
        int sum = a.charAt(i) - '0' + b.charAt(j) - '0' + c;
        c = sum / 2;
        sb.insert(0, (char)('0' + sum % 2));
        i--;
        j--;
    }
    if (i >= 0) {
        for (int k = i; k >= 0; k--) {
            int sum = a.charAt(k) - '0' + c;
            c = sum / 2;
            sb.insert(0, (char)('0' + sum % 2));
        }
    }
    if (j >= 0) {
        for (int k = j; k >= 0; k--) {
            int sum = b.charAt(k) - '0' + c;
            c = sum / 2;
            sb.insert(0, (char)('0' + sum % 2));
        }
    }
    if (c == 1) sb.insert(0, '1');
    return sb.toString();
}
```

### 问题：Multiply Strings

题号：43

```txt
Input: num1 = "2", num2 = "3"
Output: "6"
```

[思路](https://leetcode.com/problems/multiply-strings/discuss/17605/Easiest-JAVA-Solution-with-Graph-Explanation)

```java
public String multiply(String num1, String num2) {
    int m = num1.length(), n = num2.length();
    int[] pos = new int[m + n];

    for(int i = m - 1; i >= 0; i--) {
        for(int j = n - 1; j >= 0; j--) {
            int mul = (num1.charAt(i) - '0') * (num2.charAt(j) - '0');
            int p1 = i + j, p2 = i + j + 1;
            int sum = mul + pos[p2];

            pos[p1] += sum / 10;
            pos[p2] = (sum) % 10;
        }
    }  

    StringBuilder sb = new StringBuilder();
    for(int p : pos) if(!(sb.length() == 0 && p == 0)) sb.append(p);
    return sb.length() == 0 ? "0" : sb.toString();
}
```

### 问题：Pow(x, n)

题号：50

思路：对指数做移位，扩大底数

```java
public double myPow(double x, int n) {
    boolean reverse = false;
    if(n < 0) {
        n = -n;
        reverse = true;
    }
    double tmp = x;
    double res = 1;
    for(int i = 0; i < 32; i++) {
        if((n & 1) == 1) res *= tmp;
        n = n >> 1;
        tmp = tmp * tmp;
    }
    return reverse ? 1.0 / res : res;
}
```

## Tree

### 问题：Binary Tree Preorder Traversal

题号：144

递归

```java
public List<Integer> preorderTraversal(TreeNode root) {
    List<Integer> list = new ArrayList<>();
    preOrder(list, root);
    return list;
}

private void preOrder(List<Integer> list, TreeNode root) {
    if (root == null) {
        return;
    }
    list.add(root.val);
    preOrder(list, root.left);
    preOrder(list, root.right);
}
```

非递归

```java
public List<Integer> preorderTraversal(TreeNode root) {
    List<Integer> result = new ArrayList<>();
    Deque<TreeNode> stack = new ArrayDeque<>();
    TreeNode p = root;
    while(!stack.isEmpty() || p != null) {
        if(p != null) {
            stack.push(p);
            result.add(p.val);  // Add before going to children
            p = p.left;
        } else {
            TreeNode node = stack.pop();
            p = node.right;
        }
    }
    return result;
}
```

### 问题：Binary Tree Inorder Traversal

题号：94

递归：

```java
public List<Integer> inorderTraversal(TreeNode root) {
    List<Integer> list = new ArrayList<>();
    inOrder(list, root);
    return list;
}

private void inOrder(List<Integer> list, TreeNode root) {
    if (root == null) {
        return;
    }
    inOrder(list, root.left);
    list.add(root.val);
    inOrder(list, root.right);
}
```

非递归：

```java
public List<Integer> inorderTraversal(TreeNode root) {
    List<Integer> result = new ArrayList<>();
    Deque<TreeNode> stack = new ArrayDeque<>();
    TreeNode p = root;
    while(!stack.isEmpty() || p != null) {
        if(p != null) {
            stack.push(p);
            p = p.left;
        } else {
            TreeNode node = stack.pop();
            result.add(node.val);  // Add after all left children
            p = node.right;
        }
    }
    return result;
}
```

### Binary Tree Postorder Traversal

题号：145

递归：

```java
public List<Integer> postorderTraversal(TreeNode root) {
    List<Integer> list = new ArrayList<>();
    postOrder(list, root);
    return list;
}

private void postOrder(List<Integer> list, TreeNode root) {
    if (root == null) {
        return;
    }
    postOrder(list, root.left);
    postOrder(list, root.right);
    list.add(root.val);
}
```

非递归：

```java
public List<Integer> postorderTraversal(TreeNode root) {
    LinkedList<Integer> result = new LinkedList<>();
    Deque<TreeNode> stack = new ArrayDeque<>();
    TreeNode p = root;
    while(!stack.isEmpty() || p != null) {
        if(p != null) {
            stack.push(p);
            result.addFirst(p.val);  // Reverse the process of preorder
            p = p.right;             // Reverse the process of preorder
        } else {
            TreeNode node = stack.pop();
            p = node.left;           // Reverse the process of preorder
        }
    }
    return result;
}
```

### Binary Tree Level Order Traversal

题号：102

```java
public List<List<Integer>> levelOrder(TreeNode root) {
    List<List<Integer>> ans = new ArrayList<List<Integer>>();
    Deque<TreeNode> deque = new ArrayDeque<>();
    if (root == null) return ans;

    deque.offerLast(root);
    while (!deque.isEmpty()) {
        int level = deque.size();
        List<Integer> sublist = new ArrayList<>();
        for (int i = 0; i < level; i++) {
            if (deque.peekFirst().left != null) {
                deque.offerLast(deque.getFirst().left);
            }
            if (deque.peekFirst().right != null) {
                deque.offerLast(deque.getFirst().right);
            }
            sublist.add(deque.removeFirst().val);
        }
        ans.add(sublist);
    }
    return ans;
}
```

### 问题：Same Tree - Symmetric Tree

题号：100, 101

```java
public boolean isSameTree(TreeNode p, TreeNode q) {
    if (p == null && q == null) return true;
    if (p == null || q == null) return false;
    return (isSameTree(p.left, q.left) && p.val == q.val && isSameTree(p.right, q.right));
}
```

```java
public boolean isSymmetric(TreeNode root) {
    if (root == null) return true;
    TreeNode p = root.left;
    TreeNode q = root.right;
    return isSymmetircImpl(p, q);
}

private boolean isSymmetircImpl(TreeNode left, TreeNode right) {
    if (left == null && right == null) return true;
    if (left == null || right == null) return false;
    return isSymmetircImpl(left.left, right.right) && isSymmetircImpl(left.right, right.left) && left.val == right.val;
}
```

非递归

```java
private boolean isSymmetric(TreeNode root){
    if(root == null) return true;
    Stack<TreeNode> stack = new Stack<TreeNode>();
    stack.push(root.left);
    stack.push(root.right);
    TreeNode left, right;

    while(!stack.empty()){
        left = stack.pop();
        right = stack.pop();

        if(left == null && right == null){
            continue;
        }
        if(left==null || right == null || left.val != right.val){
            return false;
        }
        stack.push(left.left);
        stack.push(right.right);
        stack.push(left.right);
        stack.push(right.left);
    }
    return true;
}
```

### Invert Binary Tree

题号：226

递归：

```java
public TreeNode invertTree(TreeNode root) {
    if (root == null) {
        return null;
    }
    TreeNode right = invertTree(root.right);
    TreeNode left = invertTree(root.left);
    root.left = right;
    root.right = left;
    return root;
}
```

非递归：

```java
public TreeNode invertTree(TreeNode root) {
    if (root == null) return null;
    Queue<TreeNode> queue = new LinkedList<TreeNode>();
    queue.add(root);
    while (!queue.isEmpty()) {
        TreeNode current = queue.poll();
        TreeNode temp = current.left;
        current.left = current.right;
        current.right = temp;
        if (current.left != null) queue.add(current.left);
        if (current.right != null) queue.add(current.right);
    }
    return root;
}
```

### Path Sum I II

题号：112, 113

112:

```java
public boolean hasPathSum(TreeNode root, int sum) {
    if (root == null) return false;
    if (root.left == null && root.right == null && sum - root.val == 0) return true;
    return hasPathSum(root.left, sum - root.val) || hasPathSum(root.right, sum - root.val);
}
```

113:

递归：

```java
public List<List<Integer>> pathSum(TreeNode root, int sum) {
    List<List<Integer>> ans = new ArrayList<List<Integer>>();
    pathSumImpl(root, ans, new ArrayList<>(), 0, sum);
    return ans;
}

private void pathSumImpl(TreeNode root, List<List<Integer>> res, List<Integer> sublist, int sum, int target) {
    if (root == null) {
        return;
    }
    sublist.add(root.val);
    sum += root.val;
    if (root.left == null && root.right == null && sum == target) {
        res.add(new ArrayList<>(sublist));
    }
    pathSumImpl(root.left, res, sublist, sum, target);
    pathSumImpl(root.right, res, sublist, sum, target);
    sublist.remove(sublist.size() - 1);
}
```

### 问题：二叉树最小深度，最大深度

题号：111，104

递归：

```java
public int minDepth(TreeNode root) {
    if(root == null) return 0;
    int left = minDepth(root.left);
    int right = minDepth(root.right);
    return (left == 0 || right == 0) ? left + right + 1: Math.min(left,right) + 1;
}
```

```java
public int maxDepth(TreeNode root) {
    if (root == null) return 0;
    int left = maxDepth(root.left);
    int right = maxDepth(root.right);
    return Math.max(left, right) + 1;
}
```

### 问题：Binary Tree Maximum Path Sum

题号：124

```java
int max = Integer.MIN_VALUE;
public int maxPathSum(TreeNode root) {
    sumToFather(root);
    return max;
}

private int sumToFather(TreeNode root) {
    if (root == null) return 0;
    int left = Math.max(0, sumToFather(root.left));
    int right = Math.max(0, sumToFather(root.right));
    max = Math.max(max, left + right + root.val);
    return Math.max(left, right) + root.val;
}
```

### 问题：Binary Tree Right Side View

题号：199

```java
public List<Integer> rightSideView(TreeNode root) {
        List<Integer> result = new ArrayList();
        if(root == null)
            return result;
        Queue<TreeNode> que = new LinkedList();
        que.add(root);
        while(!que.isEmpty()){
            int size = que.size();
            while(size>0){
                TreeNode node = que.poll();
                // 只取每一层最后一个
                if(size==1)
                    result.add(node.val);
                if(node.left != null)
                    que.add(node.left);
                if(node.right != null)
                    que.add(node.right);
                size--;
            }
        }
        return result;
    }
```

### 问题：Validate Binary Search Tree

题号：98

```java
public boolean isValidBST(TreeNode root) {
    return isValidBST(root, Long.MIN_VALUE, Long.MAX_VALUE);
}

private boolean isValidBST(TreeNode root, long minVal, long maxVal) {
    if (root == null) return true;
    if (root.val >= maxVal || root.val <= minVal) return false;
    return isValidBST(root.left, minVal, root.val) && isValidBST(root.right, root.val, maxVal);
}
```

### 问题：Convert Sorted Array/List to Binary Search Tree

题号：108，109

```java
public TreeNode sortedArrayToBST(int[] nums) {
    return buildTree(nums, 0, nums.length - 1);
}

private TreeNode buildTree(int[] nums, int low, int high) {
    if (low > high)
        return null;
    int mid = (low + high) / 2;

    TreeNode root = new TreeNode(nums[mid]);
    root.left = buildTree(nums, low, mid - 1);
    root.right = buildTree(nums, mid + 1, high);
    return root;
}
```

```java
public TreeNode sortedListToBST(ListNode head) {
    if (head == null) return null;
    return toBST(head, null);
}
private TreeNode toBST(ListNode head, ListNode tail) {
    ListNode slow = head;
    ListNode fast = head;
    if (head == tail) return null;

    // 快指针指到链表尾部的时候，慢指针指到链表中部
    while (fast != tail && fast.next != tail) {
        fast = fast.next.next;
        slow = slow.next;
    }

    TreeNode thead = new TreeNode(slow.val);
    thead.left = toBST(head, slow);
    thead.right = toBST(slow.next, tail);
    return thead;
}
```