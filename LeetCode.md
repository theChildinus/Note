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
            i++;
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
    for (int i = 1; i < nums.length; i++) {
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
    // i < 2 用于处理数组元素少于等于2的情况
        if (i < 2 || n > nums[i-2])
            nums[i++] = n;
    return i;
}
```

### 问题：Find All Duplicates in an Array

思路一：将元素交换到相应的位置上

```java
public List<Integer> findDuplicates(int[] nums) {
    if (nums.length == 0) {
        return new ArrayList<>();
    }
    List<Integer> ans = new ArrayList<>();
    int i = 0;
    while (i < nums.length) {
        if (nums[nums[i] - 1] != nums[i]) {
            int tmp = nums[nums[i] - 1];
            nums[nums[i] - 1] = nums[i];
            nums[i] = tmp;
        } else {
            i++;
        }
    }
    for (int j = 0; j < nums.length; j++) {
        if (nums[j] != j + 1) {
            ans.add(nums[j]);
        }
    }
    return ans;
}
```

思路2：当找到数字i时，将位置i-1处的数字翻转为负数。如果位置i-1处的数字已经为负数，则i是出现两次的数字。

```java
public List<Integer> findDuplicates(int[] nums) {
    List<Integer> result = new ArrayList<Integer>();
    for(int i = 0; i < nums.length; i++){
        int index = Math.abs(nums[i]) - 1;
        if(nums[index] > 0)
            nums[index] = -nums[index];
        else
            result.add(Math.abs(nums[i]));
    }
    return result;
}
```

### 问题：旋转数组

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
    reverse(nums, k, nums.length - 1);
}
```

```java
// 左旋，向左移动
public void rotate(int[] nums, int k) {
    k %= nums.length;
    reverse(nums, 0, k - 1);
    reverse(nums, k, nums.length - 1);
    reverse(nums, 0, nums.length - 1);
}
```

### 问题：从未排序的整数数组中找出最小的未出现的正整数

题号：41

思路：将每个数字放到正确的位置上，然后从前往后遍历，找到第一个数组元素与下标不相等的地方，返回 place ＋ 1

```java
    public int firstMissingPositive(int[] nums) {
        for (int i = 0; i < nums.length; i++) {
            // nums[nums[i] - 1] 中，nums[i] - 1 为该元素应该存放的下标，同时处理了重复的情况
            while (nums[i] > 0 && nums[i] <= nums.length && nums[i] != nums[nums[i] - 1]) {
                // 将数字1 放到下标为0的位置，依此类推
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

### 问题：Median of Two Sorted Arrays

题号：4

思想：[分治](https://blog.csdn.net/hk2291976/article/details/**51107778**)

```java
public double findMedianSortedArrays(int[] nums1, int[] nums2) {
    int n = nums1.length;
    int m = nums2.length;
    if(n > m)   //保证数组1一定最短
        return findMedianSortedArrays(nums2, nums1);
    int L1 = 0,L2 = 0, R1 = 0, R2 = 0;
    int c1, c2, lo = 0, hi = 2*n;  //我们目前是虚拟加了'#'所以数组1是2*n长度
    while(lo <= hi)   //二分
    { 
        c1 = (lo + hi) / 2;  //c1是二分的结果
        c2 = m + n - c1;
        L1 = (c1 == 0) ? Integer.MIN_VALUE : nums1[(c1 - 1) / 2];   //map to original element
        R1 = (c1 == 2 * n) ? Integer.MAX_VALUE : nums1[c1 / 2];
        L2 = (c2 == 0) ? Integer.MIN_VALUE : nums2[(c2 - 1) / 2];
        R2 = (c2 == 2 * m) ? Integer.MAX_VALUE : nums2[c2 / 2];

        if(L1 > R2)
            hi = c1 - 1;
        else if(L2 > R1)
            lo = c1 + 1;
        else
            break;
    }
    return (Math.max(L1, L2) + Math.min(R1, R2)) / 2.0;
}
```

### 问题：Merge/Insert Intervals

题号：56, 57

```java
public List<Interval> merge(List<Interval> intervals) {
    if (intervals.isEmpty()) return new ArrayList<Interval>();
    Collections.sort(intervals, new Comparator<Interval>() {
        @Override
        public int compare(Interval o1, Interval o2) {
            return Integer.compare(o1.start, o2.start);
        }
    });
    List<Interval> ans = new LinkedList<>();
    int start = intervals.get(0).start;
    int end = intervals.get(0).end;
    for (Interval interval : intervals) {
        if (interval.start <= end) {
            end = Math.max(interval.end, end);
        } else {
            ans.add(new Interval(start, end));
            start = interval.start;
            end = interval.end;
        }
    }
    ans.add(new Interval(start, end));
    return ans;
}
```

### 问题：Maximum Subarray

题号：53

思路：顺序相加求和，如果和比当前元素小，从当前元素开始求和，用result保存每次求和结果中的最大值

```java
public int maxSubArray(int[] nums) {
    int result = Integer.MIN_VALUE;
    int sum = 0;
    for (int i = 0; i < nums.length; i++) {
        sum = Math.max(sum + nums[i], nums[i]);
        result = Math.max(result, sum);
    }
    return result;
}
```

思路2：DP

```java
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

### 问题：Search in Rotated Sorted Array I II

题号：33 I：无重复元素

```java
public int search(int[] nums, int target) {
    if (nums.length == 0) {
        return -1;
    }
    int lo = 0;
    int hi = nums.length - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (nums[mid] == target) return mid;
        // 这里判断的是 nums[low] 和 nums[mid]，而不是和target比较
        // nums[start..mid] is sorted
        if (nums[lo] <= nums[mid]) {
            if (target >= nums[lo] && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else { // nums[mid..end] is sorted
            if (target > nums[mid] && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return nums[lo] == target ? lo : -1;
}
```

题号：81 II：含有重复元素

```java
public boolean search(int[] nums, int target) {
    if (nums.length == 0) return false;
    int low = 0;
    int high = nums.length - 1;
    while (low < high) {
        int mid = (low + high) / 2;
        if (nums[mid] == target) {
            return true;
        }
        if (nums[low] < nums[mid]) {
            if (target >= nums[low] && target < nums[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        } else if (nums[low] > nums[mid]) {
            if (target > nums[mid] && target <= nums[high]) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        } else if (nums[low] == nums[mid]) {
            // 根据题意，较大的元素只可能出现在前半部分，不可能存在后半部分
            // II 与 I的主要区别在此，最坏情况O(n)
            low++;
        }
    }
    return nums[low] == target;
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

思路：[链接](https://leetcode.com/problems/remove-duplicate-letters/discuss/76762/Java-O(n)-solution-using-stack-with-detail-explanation)

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

Set
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

## **Math**

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

### 问题 Power of Four

```java
public boolean isPowerOfFour(int num) {
    if(num == 0) return false;
    if(num == 1) return true;
    return (num % 4 == 0) && isPowerOfFour(num / 4);
}
```

## **Tree**

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

### 问题：Unique Binary Search Trees I II

题号：96 95

96 描述：求二叉搜索树的个数

思路：`F(i, n)` 表示以某个节点i为根节点，小于i的节点为左子树，大于i的节点为右子树可以构造的树的个数，`F(i, n) = G(i - 1) * G(n - i)`, `G(i)` 表示当树节点个数为i时，可以构造树的个数 `G(n) += sum(F(i, n))`

```java
public int numTrees(int n) {
int[] G = new int[n + 1];
G[0] = 1;
G[1] = 1;

for (int i = 2; i <= n; ++i) {
    for (int j = 1; j <= i; ++j) {
    G[i] += G[j - 1] * G[i - j];
    }
}
return G[n];
}
```

95 描述：求出所有的二叉搜索树

```java
public List<TreeNode> generateTrees(int n) {
    if (n <= 0) return new ArrayList<>();
    List<TreeNode> ans = new ArrayList<>();
    return buildSubTree(1, n);
}

private List<TreeNode> buildSubTree(int start, int end) {
    List<TreeNode> res = new ArrayList<>();
    if (end < start) {
        res.add(null);
        return res;
    }

    for (int i = start; i <= end; i++) {
        List<TreeNode> leftNodes = buildSubTree(start, i - 1);
        List<TreeNode> rightNodes = buildSubTree(i + 1, end);
        for (TreeNode left : leftNodes) {
            for (TreeNode right : rightNodes) {
                TreeNode root = new TreeNode(i);
                root.left = left;
                root.right = right;
                res.add(root);
            }
        }
    }
    return res;
}
```

## **BackTracking**

### 问题：Subsets I，II

题号：78，90

思路：回溯法，依次添加元素，在返回之前删除list尾部的元素

回溯法的基本思想是按照输入数组的顺序，每一层递归处理一个元素，当处理到最后一层的时候，也就是把数组中的所有元素都处理完的时候，把当前结果加入到最后的返回结果中。值得注意的是，每次在递归到下一层之前，我们加入了某个要处理的元素X，在下一层递归返回之后，我们要把之前加入的元素X从当前结果中取出来。如果我们不把元素X取出来，那么在下一次循环中，我们还会加入新的元素Y。那么在这一层递归中就相当于处理了不止一个新元素。

回溯法对输入队列是否要排序取决于：

- 输出队列是否有序
- 去重目的，将后续出现的同一数字的排列情况删去

```java
public List<List<Integer>> subsets(int[] nums) {
    if (nums.length == 0) return new ArrayList<>();
    List<List<Integer>> ans = new ArrayList<>();
    Arrays.sort(nums);
    backTrack(ans, new ArrayList<>(), nums, 0);
    return ans;
}

private void backTrack(List<List<Integer>> list, List<Integer> tmplist, int[] nums, int start) {
    list.add(new ArrayList<>(tmplist));
    for (int i = start; i < nums.length; i++) {
        tmplist.add(nums[i]);
        backTrack(list, tmplist, nums, i + 1);
        tmplist.remove(tmplist.size() - 1);
    }
}
```

90:

```java
public List<List<Integer>> subsetsWithDup(int[] nums) {
    if (nums.length == 0) return new ArrayList<>();
    List<List<Integer>> ans = new ArrayList<>();
    Arrays.sort(nums);
    subsetbackTrack(ans, new ArrayList<>(), nums, 0);
    return ans;
}

private void subsetbackTrack(List<List<Integer>> list, List<Integer> tmplist, int[] nums, int start) {
    list.add(new ArrayList<>(tmplist));

    for (int i = start; i < nums.length; i++) {
        //  如果后一个元素和之前的元素相同，则不再操作，避免重复
        if (i > start && nums[i] == nums[i - 1]) continue;
        tmplist.add(nums[i]);
        subsetbackTrack(list, tmplist, nums, i + 1);
        tmplist.remove(tmplist.size() - 1);
    }
}
```

问题：Combination Sum I II III

题号：39，40，216

39:

```txt
Input: candidates = [2,3,6,7], target = 7,
A solution set is:
[
  [7],
  [2,2,3]
]
```

```java
public List<List<Integer>> combinationSum(int[] nums, int target) {
    List<List<Integer>> list = new ArrayList<>();
    Arrays.sort(nums);
    combineBackTrack1(list, new ArrayList<>(), nums, target, 0);
    return list;
}

private void combineBackTrack1(List<List<Integer>> list, List<Integer> tempList, int [] nums, int remain, int start){
    if(remain < 0) return;
    else if(remain == 0) {
        list.add(new ArrayList<>(tempList));
        return;
    }

    for(int i = start; i < nums.length; i++){
        tempList.add(nums[i]);
        // not i + 1 because we can reuse same elements
        combineBackTrack1(list, tempList, nums, remain - nums[i], i);
        tempList.remove(tempList.size() - 1);
    }
}
```

40：

```txt
Input: candidates = [10,1,2,7,6,1,5], target = 8,
A solution set is:
[
  [1, 7],
  [1, 2, 5],
  [2, 6],
  [1, 1, 6]
]
```

```java
public List<List<Integer>> combinationSum2(int[] candidates, int target) {
    if (candidates.length == 0) return new ArrayList<>();
    int[] nums = candidates;
    Arrays.sort(nums);
    List<List<Integer>> ans = new ArrayList<>();
    combineBackTrack2(ans, new ArrayList<>(), nums, 0, target);
    return ans;
}

private void combineBackTrack2(List<List<Integer>> ans, List<Integer> tmplist, int[] nums, int start, int target) {
    if (target == 0) {
        ans.add(new ArrayList<>(tmplist));
        return;
    }
    if (target < 0) return;

    for (int i = start; i < nums.length; i++) {
        if (i > start && nums[i] == nums[i - 1]) continue;
        tmplist.add(nums[i]);
        combineBackTrack2(ans, tmplist, nums, i + 1, target - nums[i]);
        tmplist.remove(tmplist.size() - 1);
    }
}
```

216：

```txt
Input: k = 3, n = 9
Output: [[1,2,6], [1,3,5], [2,3,4]]
```

```java
public List<List<Integer>> combinationSum3(int k, int n) {
    if (k == 0 || n == 0) return new ArrayList<>();
    int[] nums = new int[9];
    for (int i = 0; i < 9; i++) {
        nums[i] = i + 1;
    }
    List<List<Integer>> ans = new ArrayList<>();
    combineBackTrack3(ans, new ArrayList<>(), nums, 0, k, n);
    return ans;
}

public void combineBackTrack3(List<List<Integer>> ans, List<Integer> tmplist, int[] nums, int start, int k, int n) {
    if (tmplist.size() == k && n == 0) {
        ans.add(new ArrayList<>(tmplist));
        return;
    }

    for (int i = start; i < nums.length; i++) {
        tmplist.add(nums[i]);
        combineBackTrack3(ans, tmplist, nums, i + 1, k, n - nums[i]);
        tmplist.remove(tmplist.size() - 1);
    }
}
```

### 问题：Permutations 全排列 I II

题号：46 47 31

```java
public List<List<Integer>> permute(int[] nums) {
    if (nums.length == 0) return new ArrayList<>();
    List<List<Integer>> ans = new ArrayList<>();
    permuteBackTrack(ans, new ArrayList<>(), nums);
    return ans;
}

private void permuteBackTrack(List<List<Integer>> ans, List<Integer> sublist, int[] nums) {
    if (sublist.size() == nums.length) {
        ans.add(new ArrayList<>(sublist));
        return;
    }
    // i从0开始
    for (int i = 0; i < nums.length; i++) {
        if (sublist.contains(nums[i])) continue;
        sublist.add(nums[i]);
        permuteBackTrack(ans, sublist, nums);
        sublist.remove(sublist.size() - 1);
    }
}
```

47: 有重复元素的全排列

```java
public List<List<Integer>> permuteUnique(int[] nums) {
    List<List<Integer>> list = new ArrayList<>();
    Arrays.sort(nums);
    backtrack(list, new ArrayList<>(), nums, new boolean[nums.length]);
    return list;
}

private void backtrack(List<List<Integer>> list, List<Integer> tempList, int [] nums, boolean [] used){
    if(tempList.size() == nums.length){
        list.add(new ArrayList<>(tempList));
    } else{
        for(int i = 0; i < nums.length; i++){
            if(used[i] || i > 0 && nums[i] == nums[i-1] && !used[i - 1]) continue;
            used[i] = true;
            tempList.add(nums[i]);
            backtrack(list, tempList, nums, used);
            used[i] = false;
            tempList.remove(tempList.size() - 1);
        }
    }
}
```

31：求下一个全排列

[参考讲解](https://blog.csdn.net/NoMasp/article/details/49913627)

[演示](https://leetcode.com/problems/next-permutation/solution/)

数学中的排列组合顺序：数值是依次增大的

```txt
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
```

对于 `6 5 4 8 7 5 1` 的下一个排列应该是 `6 5 5 1 4 7 8`

```java
public void nextPermutation(int[] nums) {
    int i = nums.length - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) i--;
    if (i >= 0) {
        int j = nums.length - 1;
        while (j >= 0 && nums[j] <= nums[i]) j--;
        swap(nums, i, j);
    }
    reverse(nums, i + 1);
}
private void reverse(int[] nums, int start) {
    int i = start;
    int j = nums.length - 1;
    while (i < j) {
        swap(nums, i, j);
        i++;
        j--;
    }
}
private void swap(int[] nums, int i, int j) {
    int tmp = nums[i];
    nums[i] = nums[j];
    nums[j] = tmp;
}
```

### 问题：Letter Combinations of a Phone Number

题号：17

暴力：

```java
public List<String> letterCombinations(String digits) {
    if (digits.isEmpty()) return new ArrayList<String>();

    List<String> ans = Letter2List(0, digits);
    for (int i = 1; i < digits.length(); i++) {
        List<String> next = Letter2List(i, digits);
        List<String> tmp = combine(ans, next);
        ans = tmp;
    }
    return ans;
}

private List<String> Letter2List(int key, String digits) {
    List<String> phone = new ArrayList<>(Arrays.asList(" ", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"));
    String s = phone.get(digits.charAt(key) - '0');
    List<String> ret = new ArrayList<>();
    for (int i = 0; i < s.length(); i++) {
        ret.add(String.valueOf(s.charAt(i)));
    }
    return ret;
}

private List<String> combine(List<String> l1, List<String> l2) {
    List<String> ret = new ArrayList<>();
    for (String ll1 : l1) {
        for (String ll2 : l2) {
            String tmp = ll1 + ll2;
            ret.add(tmp);
        }
    }
    return ret;
}
```

回溯：

```java
public List<String> letterCombinations(String digits) {
    //把table上的数字对应的字母列出来，当输入为2是，digits[2]就是2所对应的"abc"
    String[] table = new String[]{"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
    List<String> list = new ArrayList<String>();
    //index从0开始，即digits的第一个数字
    letterCombinations(list, digits, "", 0, table);
    return list;
}

// digits中每个数字为一层

private void letterCombinations (List<String> list, String digits, String curr, int index, String[] table) {
    //最后一层退出条件
    if (index == digits.length()) {
        if(curr.length() != 0) list.add(curr);
        return;
    }

    //找到数字对应的字符串
    String temp = table[digits.charAt(index) - '0'];
    for (int i = 0; i < temp.length(); i++) {
        //每次循环把不同字符串加到当前curr之后
        String next = curr + temp.charAt(i);
        //进入下一层
        letterCombinations(list,digits,next,index+1,table);
    }
}
```

### 问题：Subtree of another tree

题号：572

```java
public boolean isSubtree(TreeNode s, TreeNode t) {
    if(s==null)
        return false;
    else {
        if(s.val == t.val && isSameTree(s, t))
            return true;
        else
            return isSubtree(s.left, t) || isSubtree(s.right, t);
    }
}

public boolean isSameTree(TreeNode n1, TreeNode n2) {
    if(n1==null && n2==null)
        return true;
    else if(n1==null || n2==null)
        return false;
    else {
        return  n1.val==n2.val &&
                isSameTree(n1.left, n2.left) && 
                isSameTree(n1.right, n2.right);
    }
}
```

### 问题：Flatten Binary Tree to Linked List

题号：114

思路：先找到最左子节点left，其父节点为father，father的右节点为right，先断开father和right，将left连到father的右侧，原left置null，right连接到新right右侧

```txt
     1
    / \
   2   5
  / \   \
 3   4   6

     1
    / \
   2   5
    \   \
     3   6
      \
       4

   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6
```

```java
public void flatten(TreeNode root) {
    if (root == null) return;
    if (root.left != null) flatten(root.left);
    if (root.right != null) flatten(root.right);

    TreeNode tmp = root.right;
    root.right = root.left;
    root.left = null;
    while (root.right != null) {
        root = root.right;
    }
    root.right = tmp;
}
```

## **Dynamic Programming**

### 问题：Climbing Stairs

题号：70

思路1: Fibonacci Number

```java
public int climbStairs(int n) {
    if (n == 1) {
        return 1;
    }
    int first = 1;
    int second = 2;
    for (int i = 3; i <= n; i++) {
        int third = first + second;
        first = second;
        second = third;
    }
    return second;
}
```

思路2: dp

```java
public int climbStairs(int n) {
    if (n == 1) {
        return 1;
    }
    int[] dp = new int[n + 1];
    dp[1] = 1;
    dp[2] = 2;
    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}
```

### 问题：Triangle

题号：120

down-top

```java
public int minimumTotal(List<List<Integer>> triangle) {
    if (triangle == null || triangle.isEmpty()) return 0;

    int rows = triangle.size();
    int[] sums = new int[rows + 1];

    for (int i = rows - 1; i >= 0 ; i--) {
        List<Integer> rowNodes = triangle.get(i);
        for (int j = 0; j <= i; j++) {
            sums[j] = rowNodes.get(j) + Math.min(sums[j], sums[j + 1]);
        }
    }

    return sums[0];
}
```

top-down

```java
public int minimumTotal(List<List<Integer>> triangle) {
    if (triangle.size() == 0) return 0;
    List<Integer> ans = triangle.get(0);
    for (int i = 1; i < triangle.size(); i++) {
        ans = addLevel(ans, triangle.get(i));
    }
    int min = Integer.MAX_VALUE;
    for (int i : ans) {
        min = Math.min(min, i);
    }
    return min;
}

private List<Integer> addLevel(List<Integer> up, List<Integer> down) {
    List<Integer> ret = new ArrayList<Integer>();
    for (int i = 0; i < down.size(); i++) {
        if (i == 0) {
            ret.add(up.get(0) + down.get(0));
        } else if (i == down.size() - 1) {
            ret.add(up.get(up.size() - 1) + down.get(down.size() - 1));
        } else {
            ret.add(Math.min(down.get(i) + up.get(i), down.get(i) + up.get(i - 1)));
        }
    }
    return ret;
}
```

### 问题：Word Break

题号：139

思路：

- `dp[i]` 表示前 `i` 个字符能不能被dict完美划分
- 判断 `di[i]` 需要遍历 `0~i` 中是否存在一个j使得 `dp[j] == true` 而且 `j+1 ~ i` 存在于dict中
- 注意是前 `i` 个字符 `substring(j, i)` i代表的是尾后指针

```java
public boolean wordBreak(String s, List<String> wordDict) {
    if (s.isEmpty()) return false;
    int n = s.length();
    boolean[] dp = new boolean[n + 1];
    dp[0] = true;
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < i; j++) {
            if (dp[j] && wordDict.contains(s.substring(j, i))) {
                dp[i] = true;
            }
        }
    }
    return dp[n];
}
```

### 问题：Perfect Squares

题号：279

思路：

```txt
dp[0] = 0
dp[1] = dp[0]+1 = 1
dp[2] = dp[1]+1 = 2
dp[3] = dp[2]+1 = 3
dp[4] = Min{ dp[4-1*1]+1, dp[4-2*2]+1 }
      = Min{ dp[3]+1, dp[0]+1 }
      = 1
dp[5] = Min{ dp[5-1*1]+1, dp[5-2*2]+1 }
      = Min{ dp[4]+1, dp[1]+1 }
      = 2
                        .
                        .
                        .
dp[13] = Min{ dp[13-1*1]+1, dp[13-2*2]+1, dp[13-3*3]+1 }
       = Min{ dp[12]+1, dp[9]+1, dp[4]+1 }
       = 2
                        .
                        .
                        .
dp[n] = Min{ dp[n - i*i] + 1 },  n - i*i >=0 && i >= 1
```

```java
public int numSquares(int n) {
    int[] dp = new int[n + 1];
    Arrays.fill(dp, Integer.MAX_VALUE);
    dp[0] = 0;
    for(int i = 1; i <= n; ++i) {
        int min = Integer.MAX_VALUE;
        int j = 1;
        while(i - j*j >= 0) {
            // 1代表取平方数
            min = Math.min(min, dp[i - j*j] + 1);
            ++j;
        }
        dp[i] = min;
    }
    return dp[n];
}
```

### 问题：Dungeon Game

题号：174

从后向前依次决定每个位置至少需要多少血量，状态转移方程 `dp[i][j] = min(dp[i+1][j] - dungon[i][j], dp[i][j+1] - dungon[i][j])`

还有每个状态的血量都要至少为1，因此我们还需要在上一步的基础上保证`dp[i][j] = max(dp[i][j], 1)`

```java
public int calculateMinimumHP(int[][] dungon) {
    if (dungon == null || dungon.length == 0 || dungon[0].length  == 0) return 0;

    int m = dungon.length;
    int n = dungon[0].length;

    int[][] health = new int[m][n];

    health[m - 1][n - 1] = Math.max(1 - dungon[m - 1][n - 1], 1);

    for (int i = m - 2; i >= 0; i--) {
        health[i][n - 1] = Math.max(health[i + 1][n - 1] - dungon[i][n - 1], 1);
    }

    for (int j = n - 2; j >= 0; j--) {
        health[m - 1][j] = Math.max(health[m - 1][j + 1] - dungon[m - 1][j], 1);
    }

    for (int i = m - 2; i >= 0; i--) {
        for (int j = n - 2; j >= 0; j--) {
            int down = Math.max(health[i + 1][j] - dungon[i][j], 1);
            int right = Math.max(health[i][j + 1] - dungon[i][j], 1);
            health[i][j] = Math.min(down, right);
        }
    }
    return health[0][0];
}
```

状态压缩

```java
public int calculateMinimumHP(int[][] dungon) {
    if (dungon == null || dungon.length == 0 || dungon[0].length  == 0) return 0;

    int m = dungon.length;
    int n = dungon[0].length;

    int[] dp = new int[n + 1];
    Arrays.fill(dp, Integer.MAX_VALUE);
    dp[n - 1] = 1;
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            // 这里不能写成 min(dp[j] - dungon[i][j], d[j + 1] - dungon[i][j])，因为dp[j + 1] = Integer.MAX_VALUE，dungon[i][j]为负数时会超范围
            dp[j] = Math.max(Math.min(dp[j], dp[j + 1]) - dungon[i][j], 1);
        }
    }
    return dp[0];
}
```

### 问题：Range Sum Query 2D - Immutable

题号：304

[思路](https://leetcode.com/problems/range-sum-query-2d-immutable/discuss/75350/Clean-C++-Solution-and-Explaination-O(mn)-space-with-O(1)-time)

```java
class NumMatrix {
    private int[][] dp;
    public NumMatrix(int[][] matrix) {
        if (matrix == null || matrix.length == 0) {
            return;
        }
        int m = matrix.length;
        int n = matrix[0].length;
        dp = new int[m + 1][n + 1];

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1] - dp[i - 1][j - 1] + matrix[i - 1][j - 1];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        if (dp == null || dp.length == 0) {
            return 0;
        }
        return dp[row2 + 1][col2 + 1] - dp[row2 + 1][col1] - dp[row1][col2 + 1] + dp[row1][col1];
    }
}
```

## **LinkedList**

### 问题：Reverse Linked List I II

题号：206, 92

反向指，we have linked list `1 → 2 → 3 → Ø`, we would like to change it to `Ø ← 1 ← 2 ← 3`.

```java
public ListNode reverseList(ListNode head) {
    ListNode prev = null;
    ListNode curr = head;
    while (curr != null) {
        ListNode nextTemp = curr.next;
        curr.next = prev;
        prev = curr;
        curr = nextTemp;
    }
    return prev;
}
```

Recursive

```java
public ListNode reverseList(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode p = reverseList(head.next);
    head.next.next = head;
    head.next = null;
    return p;
}
```

部分反转

```java
public ListNode reverseBetween(ListNode head, int m, int n) {
    if(head == null) return null;
    ListNode dummy = new ListNode(0); // create a dummy node to mark the head of this list
    dummy.next = head;
    ListNode pre = dummy; // make a pointer pre as a marker for the node before reversing
    for(int i = 0; i<m-1; i++) pre = pre.next;

    ListNode start = pre.next; // a pointer to the beginning of a sub-list that will be reversed
    ListNode then = start.next; // a pointer to a node that will be reversed

    // 1 - 2 -3 - 4 - 5 ; m=2; n =4 ---> pre = 1, start = 2, then = 3
    // dummy-> 1 -> 2 -> 3 -> 4 -> 5

    for(int i=0; i<n-m; i++)
    {
        start.next = then.next;
        then.next = pre.next;
        pre.next = then;
        then = start.next;
    }

    // first reversing : dummy->1 - 3 - 2 - 4 - 5; pre = 1, start = 2, then = 4
    // second reversing: dummy->1 - 4 - 3 - 2 - 5; pre = 1, start = 2, then = 5 (finish)

    return dummy.next;
}
```


### 问题：Linked List Cycle

题号：141

```java
public boolean hasCycle(ListNode head) {
    if (head == null || head.next == null) {
        return false;
    }
    ListNode slow = head;
    ListNode fast = head.next;
    while (slow != fast) {
        if (fast == null || fast.next == null) {
            return false;
        }
        slow = slow.next;
        fast = fast.next.next;
    }
    return true;
}
```

### 问题：Swap Node in Pairs

题号:24

自己的解法

```java
public ListNode swapPairs(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode pre = head;
    ListNode post = head.next;
    ListNode ans = post;
    ListNode prepre = null;
    while (pre != null && post != null) {
        swap(prepre, pre, post);
        prepre = pre;
        pre = pre.next;
        if (pre != null) {
            post = pre.next;
        }
    }
    return ans;
}
private void swap(ListNode prepre, ListNode pre, ListNode post) {
    pre.next = post.next;
    post.next = pre;
    if (prepre != null) {
        prepre.next = post;
    }
}
```

别人的解法

```java
public ListNode swapPairs(ListNode head) {
    // 加空头节点， 头节点的下一个节点就是链表的头节点
    ListNode dummy = new ListNode(0);
    ListNode prev = dummy;
    prev.next = head;
    while(prev.next != null && prev.next.next!= null) {
        ListNode first = prev.next;
        ListNode second = prev.next.next;
        first.next = second.next;
        second.next = first;
        prev.next = second;
        prev = first;
    }
    return dummy.next;
}
```

### 问题：Odd Even LinkedList

题号：328

思路很简单，将下标为奇数的元素放在一个链表中，偶数的元素放到另一个链表中，然后将偶数元素的链表链接在奇数元素之后，但是写一个简单无错的代码很难

自己写的：

```java
public ListNode oddEvenList(ListNode head) {
    if (head == null || head.next == null) return head;
    ListNode oddhead = head;
    ListNode evenhead = head.next;
    ListNode oddcur = oddhead;
    ListNode evencur = evenhead;
    while (evencur != null) {
        oddcur.next = evencur.next;
        if (oddcur.next != null) {
            evencur.next = oddcur.next.next;
        }
        oddcur = oddcur.next;
        evencur = evencur.next;
    }
    ListNode tmp = oddhead;
    while (tmp.next != null) {
        tmp = tmp.next;
    }
    tmp.next = evenhead;
    return oddhead;
}
```

[LeetCode Solution](https://leetcode.com/problems/odd-even-linked-list/solution/)

```java
public ListNode oddEvenList(ListNode head) {
    if (head == null) return null;
    ListNode odd = head, even = head.next, evenHead = even;
    while (even != null && even.next != null) {
        odd.next = even.next;
        odd = odd.next;
        even.next = odd.next;
        even = even.next;
    }
    odd.next = evenHead;
    return head;
}
```

### 问题：Remove Nth Node From End of List

题号：19

自己的解法 Two pass algorithm

```java
public ListNode removeNthFromEnd(ListNode head, int n) {
    ListNode dummy = new ListNode(0);
    dummy.next = head;
    int num = 0;
    ListNode cur = dummy;
    while (cur != null) {
        num++;
        cur = cur.next;
    }
    cur = dummy;
    int count = 0;
    while (cur != null && count++ != num - n - 1) {
        cur = cur.next;
    }
    cur.next = cur.next.next;
    return dummy.next;
}
```

One pass algorithm：

```java
public ListNode removeNthFromEnd(ListNode head, int n) {
    ListNode dummy = new ListNode(0);
    dummy.next = head;
    ListNode first = dummy;
    ListNode second = dummy;
    // Advances first pointer so that the gap between first and second is n nodes apart
    for (int i = 1; i <= n + 1; i++) {
        first = first.next;
    }
    // Move first to the end, maintaining the gap
    while (first != null) {
        first = first.next;
        second = second.next;
    }
    second.next = second.next.next;
    return dummy.next;
}
```

### 问题：Remove Duplicates from Sorted List I II

题号：83 82

83：

```java
public ListNode deleteDuplicates(ListNode head) {
    ListNode dummy = new ListNode(Integer.MAX_VALUE);
    dummy.next = head;
    ListNode cur = dummy;
    while (cur != null && cur.next != null) {
        if (cur.val == cur.next.val) {
            cur.next = cur.next.next;
        } else {
            cur = cur.next;
        }
    }
    return dummy.next;
}
```

82：

```java
public ListNode deleteDuplicates(ListNode head) {
//use two pointers, slow - track the node before the dup nodes,
// fast - to find the last node of dups.
    ListNode dummy = new ListNode(0), fast = head, slow = dummy;
    slow.next = fast;
    while(fast != null) {
        while (fast.next != null && fast.val == fast.next.val) {
            fast = fast.next;    //while loop to find the last node of the dups.
        }
        // 判断的是指针地址是否相同
        if (slow.next != fast) { //duplicates detected.
            slow.next = fast.next; //remove the dups.
            fast = slow.next;     //reposition the fast pointer.
        } else { //no dup, move down both pointer.
            slow = slow.next;
            fast = fast.next;
        }
    }
    return dummy.next;
```

### 问题：Add Two Numbers

题号：2

之前自己写的代码很长，优化后的版本，如果只有一个链表中含有数字，那么另一个链表中的数字假设为0，然后相加

```java
public ListNode addTwoNumbers(ListNode l1, ListNode l2) {
    if (l1 == null || l2 == null) return null;
    int c = 0;
    ListNode dummy = new ListNode(0);
    ListNode cur = dummy;
    while (l1 != null || l2 != null) {
        int x = (l1 != null) ? l1.val : 0;
        int y = (l2 != null) ? l2.val : 0;
        int sum = x + y + c;
        cur.next = new ListNode (sum % 10);
        cur = cur.next;
        if (l1 != null) l1 = l1.next;
        if (l2 != null) l2 = l2.next;
        c = sum / 10;
    }
    if (c != 0) {
        cur.next = new ListNode(c);
    }
    return dummy.next;
}
```

### 问题：Merge Two Sorted Lists

题号：21

```java
public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
    if (l1 != null && l2 == null) return l1;
    if (l1 == null && l2 != null) return l2;
    if (l1 == null && l2 == null) return null;

    ListNode dummy = new ListNode(0);
    ListNode cur = dummy;
    ListNode ll1 = l1;
    ListNode ll2 = l2;
    while (ll1 != null && ll2 != null) {
        if (ll1.val <= ll2.val) {
            cur.next = ll1;
            ll1 = ll1.next;
        } else {
            cur.next = ll2;
            ll2 = ll2.next;
        }
        cur = cur.next;
    }
    if (ll1 != null) {
        cur.next = ll1;
    } else {
        cur.next = ll2;
    }
    return dummy.next;
}
```

递归：

```java
public ListNode mergeTwoLists(ListNode l1, ListNode l2){
    if (l1 == null) return l2;
    if (l2 == null) return l1;
    if (l1.val < l2.val){
        l1.next = mergeTwoLists(l1.next, l2);
        return l1;
    } else {
        l2.next = mergeTwoLists(l1, l2.next);
        return l2;
    }
}
```

## **Matrix**

### Spiral Matrix

题号：54

自己的解法：模拟

```java
public List<Integer> spiralOrder(int[][] matrix) {
    if (matrix.length == 0 || matrix[0].length == 0) return new ArrayList<>();

    boolean[][] m = new boolean[matrix.length][matrix[0].length];
    List<Integer> ans = new ArrayList<Integer>();
    int row = matrix.length;
    int col = matrix[0].length;
    int nextrow = 0;
    int nextcol = 0;
    int direction = 0;
    while (true) {
        int i, j;
        if (direction == 0) {
            for (j = nextcol; j < col; j++) {
                if (!m[nextrow][j]) {
                    ans.add(matrix[nextrow][j]);
                    m[nextrow][j] = true;
                } else {
                    break;
                }
            }
            nextrow = nextrow + 1;
            nextcol = j - 1;
            direction = 1;
        } else if (direction == 1) {
            for (i = nextrow; i < row; i++) {
                if (!m[i][nextcol]) {
                    ans.add(matrix[i][nextcol]);
                    m[i][nextcol] = true;
                } else {
                    break;
                }
            }
            nextrow = i - 1;
            nextcol = nextcol - 1;
            direction = 2;
        } else if (direction == 2) {
            for (j = nextcol; j >= 0; j--) {
                if (!m[nextrow][j]) {
                    ans.add(matrix[nextrow][j]);
                    m[nextrow][j] = true;
                } else {
                    break;
                }
            }
            nextrow = nextrow - 1;
            nextcol = j + 1;
            direction = 3;
        } else if (direction == 3) {
            for (i = nextrow; i >= 0; i--) {
                if (!m[i][nextcol]) {
                    ans.add(matrix[i][nextcol]);
                    m[i][nextcol] = true;
                } else {
                    break;
                }
            }
            nextrow = i + 1;
            nextcol = nextcol + 1;
            direction = 0;
        }

        if (nextrow >= row || nextcol >= col ||
                nextrow < 0 || nextcol < 0 ||
                m[nextrow][nextcol]) break;

    }
    return ans;
}
```

官方优化：

```java
public List<Integer> spiralOrder(int[][] matrix) {
    List ans = new ArrayList();
    if (matrix.length == 0) return ans;
    int R = matrix.length, C = matrix[0].length;
    boolean[][] seen = new boolean[R][C];
    int[] dr = {0, 1, 0, -1};
    int[] dc = {1, 0, -1, 0};
    int r = 0, c = 0, di = 0;
    for (int i = 0; i < R * C; i++) {
        ans.add(matrix[r][c]);
        seen[r][c] = true;
        int cr = r + dr[di];
        int cc = c + dc[di];
        if (0 <= cr && cr < R && 0 <= cc && cc < C && !seen[cr][cc]){
            r = cr;
            c = cc;
        } else {
            di = (di + 1) % 4;
            r += dr[di];
            c += dc[di];
        }
    }
    return ans;
}
```

### 问题：Search a 2D matrix II

题号：240

思路1：暴力 beats 8.18%

思路2：

```java
public boolean searchMatrix(int[][] matrix, int target) {
    if (matrix == null || matrix.length == 0) return false;
    if (matrix[0] == null || matrix[0].length == 0) return false;
    int row = matrix.length - 1;
    int col = 0;
    // 从左下角开始移动
    while (row >= 0 && col < matrix[0].length) {
        if (matrix[row][col] == target) {
            return true;
        }

        if (matrix[row][col] > target) {
            row--;
        } else {
            col++;
        }
    }
    return false;
}
```

### 问题：Word Search

题号：79

思路：DFS

```java
public boolean exist(char[][] board, String word) {
    if (board.length == 0 || board[0].length == 0) return false;
    int row = board.length;
    int col = board[0].length;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (finder(board, i, j, word, 0)) {
                return true;
            }
        }
    }
    return false;
}

private boolean finder(char[][] board, int r, int c, String word, int strpos) {
    if (strpos == word.length()) {
        return true;
    }
    if (r < 0 || c < 0 || r >= board.length || c >= board[0].length) {
        return false;
    }
    if (board[r][c] != word.charAt(strpos)) {
        return false;
    }
    char record = board[r][c];
    board[r][c] = '1';
    boolean res = finder(board, r + 1, c, word, strpos+1) || finder(board, r - 1, c, word, strpos+1) ||
            finder(board, r, c + 1, word, strpos+1) || finder(board, r, c - 1, word, strpos+1);
    board[r][c] = record;
    return res;
}
```

### 问题：Rotate Image

题号：48

思路：先按照上三角或者下三角交换元素，再按照列交换元素

```java
public void rotate(int[][] matrix) {
    if (matrix.length == 0 || matrix[0].length == 0) {
        return;
    }
    int n = matrix[0].length;
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            int tmp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = tmp;
        }
    }

    for (int j = 0; j < n / 2; j++) {
        for (int i = 0; i < n; i++) {
            int tmp = matrix[i][j];
            matrix[i][j] = matrix[i][n - 1 - j];
            matrix[i][n - 1 - j] = tmp;
        }
    }
}
```

## **BFS & DFS**

### 问题：Number of Islands

题号：200

思路1：Union Fild

并差集类：

```java
class UnionFind {
    int[] father;  
    int m, n;
    int count = 0;
    UnionFind(char[][] grid) {  
        m = grid.length;  
        n = grid[0].length;  
        father = new int[m * n];  
        for (int i = 0; i < m; i++) {  
            for (int j = 0; j < n; j++) {  
                if (grid[i][j] == '1') {
                    int id = i * n + j;
                    father[id] = id;
                    count++;
                }
            }  
        }  
    }
    public void union(int node1, int node2) {  
        int find1 = find(node1);
        int find2 = find(node2);
        if(find1 != find2) {
            father[find1] = find2;
            count--;
        }
    }
    public int find (int node) {  
        if (father[node] == node) {  
            return node;
        }
        father[node] = find(father[node]);  
        return father[node];
    }
}
```

实现：

```java
int[][] distance = {{1,0},{-1,0},{0,1},{0,-1}}; // 定义四个方向
public int numIslands(char[][] grid) {  
    if (grid == null || grid.length == 0 || grid[0].length == 0)  {
        return 0;  
    }
    UnionFind uf = new UnionFind(grid);  
    int rows = grid.length;  
    int cols = grid[0].length;  
    for (int i = 0; i < rows; i++) {  
        for (int j = 0; j < cols; j++) {  
            if (grid[i][j] == '1') {  
                for (int[] d : distance) {
                    int x = i + d[0];
                    int y = j + d[1];
                    if (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == '1') {  
                        int id1 = i*cols+j;
                        int id2 = x*cols+y;
                        // 两两将点连接起来
                        uf.union(id1, id2);  
                    }  
                }  
            }  
        }  
    }  
    return uf.count;  
}
```

思路2：DFS

```java
private int n;
private int m;

public int numIslands(char[][] grid) {
    int count = 0;
    n = grid.length;
    if (n == 0) return 0;
    m = grid[0].length;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++)
            if (grid[i][j] == '1') {
                // 将与起始的1相连的1全部置为0，并计数
                DFSMarking(grid, i, j);
                ++count;
            }
    }
    return count;
}

private void DFSMarking(char[][] grid, int i, int j) {
    if (i < 0 || j < 0 || i >= n || j >= m || grid[i][j] != '1') return;
    grid[i][j] = '0';
    DFSMarking(grid, i + 1, j);
    DFSMarking(grid, i - 1, j);
    DFSMarking(grid, i, j + 1);
    DFSMarking(grid, i, j - 1);
}
```

思路3：BFS

```java
int[][] distance = {{1,0},{-1,0},{0,1},{0,-1}};

class PosPair {
    int x;
    int y;
    public PosPair(int x, int y) {
        this.x = x;
        this.y = y;
    }
    public int getX() { return x; }
    public int getY() { return y; }
}

public int numIslands(char[][] grid) {
    if (grid.length == 0 || grid[0].length == 0) {
        return 0;
    }
    int count = 0;
    Deque<PosPair> queue = new ArrayDeque<>();
    int m = grid.length;
    int n = grid[0].length;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {
                count++;
                queue.offerLast(new PosPair(i, j));
                findIsland(queue, grid, m, n);
            }
        }
    }
    return count;
}

private void findIsland(Deque<PosPair> queue, char[][] grid, int m, int n) {
    while (!queue.isEmpty()) {
        int x = queue.peekFirst().getX();
        int y = queue.peekFirst().getY();
        queue.pollFirst();
        for (int[] dir : distance) {
            int i = x + dir[0];
            int j = y + dir[1];
            if (i < 0 || j < 0 || i >= m || j >= n|| grid[i][j] == '0') continue;
            grid[i][j] = '0';
            queue.offerLast(new PosPair(i, j));
        }
    }
}
```

## Stack & priorityQueue

### 问题：Decode String

题号：394

```java
public static String decodeString(String s) {
    if (s.isEmpty()) {
        return "";
    }
    String res = "";
    Stack<Integer> numStack = new Stack<>();
    Stack<String> strStack = new Stack<>();
    int idx = 0;
    while (idx < s.length()) {
        if (Character.isDigit(s.charAt(idx))) {
            int count = 0;
            while (Character.isDigit(s.charAt(idx))) {
                count = 10 * count + (s.charAt(idx) - '0');
                idx++;
            }
            numStack.push(count);
        } else if (s.charAt(idx) == '[') {
            strStack.push(res);
            res = "";
            idx++;
        } else if (s.charAt(idx) == ']') {
            StringBuilder tmp = new StringBuilder(strStack.pop());
            int times = numStack.pop();
            for (int i = 0; i < times; i++) {
                tmp.append(res);
            }
            res = tmp.toString();
            idx++;
        } else {
            res += s.charAt(idx);
            idx++;
        }
    }
    return res;
}
```

### 问题：Basic Calculator I II

题号：224， 227

思路：

- 遇到 '(' 就把之前的结果和符号push进stack
- 遇到')'就把 当前结果 * stack中的符号，再加上stack中之前的结果.

```java
public static int calculate(String s) {
        int len = s.length();
        int sign = 1;
        int res = 0;
        Stack<Integer> stack = new Stack<>();
        for (int i = 0; i < len; i++) {
            if (Character.isDigit(s.charAt(i))) {
                int sum = s.charAt(i) - '0';
                while (i + 1 < len && Character.isDigit(s.charAt(i + 1))) {
                    sum = sum * 10 + s.charAt(i + 1) - '0';
                    i++;
                }
                res += sum * sign;
            } else if (s.charAt(i) == '+') {
                sign = 1;
            } else if (s.charAt(i) == '-') {
                sign = -1;
            } else if (s.charAt(i) == '(') {
                stack.push(res);
                stack.push(sign);
                res = 0;
                sign = 1;
            } else if (s.charAt(i) == ')') {
                res = res * stack.pop() + stack.pop();
            }
        }
        return res;
    }
```

```java
public static int calculate2(String s) {
    Stack<Integer> stack = new Stack<Integer>();
    int num = 0;
    int idx = 0;
    int len = s.length();
    char sign = '+';

    while (idx < len) {
        if (Character.isDigit(s.charAt(idx))) {
            num = num * 10 + s.charAt(idx) - '0';
        }
        if (!Character.isDigit(s.charAt(idx)) && s.charAt(idx) != ' ' || idx == len - 1) {
            if (sign == '-') {
                stack.push(-num);
            }
            if (sign == '+') {
                stack.push(num);
            }
            if (sign == '*') {
                stack.push(stack.pop() * num);
            }
            if (sign == '/') {
                stack.push(stack.pop() / num);
            }
            sign = s.charAt(idx);
            num = 0;
        }
        idx++;
    }
    int res = 0;
    for (int i : stack) {
        res += i;
    }
    return res;
}
```

### 问题：Kth Largest Element in an Array 数组中第k大的数

题号：215

思路1：排序

```java
public int findKthLargest(int[] nums, int k) {
    Arrays.sort(nums);
    return nums[nums.length - k];
}
```

思路2：最小堆

```java
public static int findKthLargest(int[] nums, int k) {

    final PriorityQueue<Integer> pq = new PriorityQueue<>();
    for(int val : nums) {
        // 不断向最小堆中添加元素
        pq.offer(val);

        if(pq.size() > k) {
            // 超过堆允许的个数时，将最小的元素移除并调整堆，堆顶仍为最小元素
            pq.poll();
        }
    }
    // 堆顶即为第k大的数
    return pq.peek();
}
```

思路3：选择算法

- 快速排序的切分保证了
  - 所有左边的元素都不大于切分元素
  - 所有右边的元素都不小于切分元素
- 实际上切分元素的所处位置，就指定了其TopK特性，即切分元素位于第h个位置，那么切分元素就是数组中的Toph
  
算法思路：

- 完成切分过程获得切分元素的位置`j`
- 若`k>j`，则对切分元素的右半边数组进行切分
- 若`k<j`，则对切分元素的左半边数组进行切分
- 重复上述过程直到`k=j`

```java
public int findKthLargest2(int[] nums, int k) {
    k = nums.length - k;
    int lo = 0;
    int hi = nums.length - 1;
    while (lo < hi) {
        final int j = partition(nums, lo, hi);
        if (j < k) {
            lo = j + 1;
        } else if (j > k) {
            hi = j - 1;
        } else {
            break;
        }
    }
    return nums[k];
}

private int partition(int[] a, int lo, int hi) {
    int i = lo;
    int j = hi + 1;
    while (true) {
        while (i < hi && a[++i] < a[lo]);
        while (j > lo && a[lo] < a[--j]);
        if (i >= j) {
            break;
        }
        exch(a, i, j);
    }
    exch(a, lo, j);
    return j;
}
private void exch(int[] a, int i, int j) {
    final int tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}
```

### 问题：Top K Frequent Elements

题号：347

思路一：计次

```java
public static List<Integer> topKFrequent(int[] nums, int k) {
    if (nums.length == 0) {
        return new ArrayList<>();
    }
    Map<Integer, Integer> map = new HashMap<>();
    for (int i = 0; i < nums.length; i++) {
        map.put(n, map.getOrDefault(n, 0) + 1);
    }
    List<Map.Entry<Integer, Integer>> list = new ArrayList<>(map.entrySet());
    Collections.sort(list, new Comparator<Map.Entry<Integer, Integer>>() {
        @Override
        public int compare(Map.Entry<Integer, Integer> o1, Map.Entry<Integer, Integer> o2) {
            return o2.getValue() - o1.getValue();
        }
    });
    List<Integer> ans = new ArrayList<>();
    for (Map.Entry<Integer, Integer> t : list) {
        if (k != 0) {
            ans.add(t.getKey());
            k--;
        }
    }
    return ans;
}
```

思路二：桶排序

```java
public List<Integer> topKFrequent(int[] nums, int k) {

    List<Integer>[] bucket = new List[nums.length + 1];
    Map<Integer, Integer> frequencyMap = new HashMap<Integer, Integer>();

    for (int n : nums) {
        frequencyMap.put(n, frequencyMap.getOrDefault(n, 0) + 1);
    }

    for (int key : frequencyMap.keySet()) {
        // 插入桶中，桶按照次数划分
        int frequency = frequencyMap.get(key);
        if (bucket[frequency] == null) {
            bucket[frequency] = new ArrayList<>();
        }
        bucket[frequency].add(key);
    }

    List<Integer> res = new ArrayList<>();

    for (int pos = bucket.length - 1; pos >= 0 && res.size() < k; pos--) {
        if (bucket[pos] != null) {
            res.addAll(bucket[pos]);
        }
    }
    return res;
}
```

## **Bit Manipulation**

### Single Number I II III

题号：136

```java
public int singleNumber(int[] nums) {
    int mask = 0;
    for(int num : nums) mask ^= num;
    return mask;
}
```

题号：137 出现三次

思路：

a, b初始状态　　 :   0   0

第一次碰见某个数x：   0   x（把x记录在b中）

第二次碰见某个数x：   x   0（把x记录在a中）

第三次碰见某个数x：   0   0（把a和b都清空，可以处理其他数）

设计出一种变换的方法让a和b按照上述变换规则，进行转换。

- b=0时碰到x，就变成x；
- b=x时再碰到x，就变成0，这个不就是异或吗？所以我们也许可以设计`b = b xor x`
- 但是当`b = 0`时再再碰到x，这时候b还是要为0，但这时候不同的是`a = x`。所以我们可以设计成：`b = (b xor x) & ~a`,  `a = (a xor x) & ~b`

```java
public int singleNumber(int[] nums) {
    int ones = 0,twos = 0;
    for (int num : nums) {
        ones = (ones ^ num) & (~twos);
        twos = (twos ^ num) & (~ones);
    }
    return ones;
}
```

题号：260

[思路](https://blog.csdn.net/camellhf/article/details/52594524)

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

### Number of 1 Bits

题号：191

```java
public int hammingWeight(int n) {
    int bits = 0;
    int mask = 1;
    for (int i = 0; i < 32; i++) {
        if ((n & mask) != 0) {
            bits++;
        }
        mask <<= 1;
    }
    return bits;
}
```

### Reverse Bit

题号：190

```java
public int reverseBits(int n) {
    int result = 0;
    for (int i = 0; i < 32; i++) {
        result <<= 1;
        if ((n & 1) == 1) result++;
        n >>= 1;
    }
    return result;
}
```

### Missing Number

题号：268

[思路：Approach 3](https://leetcode.com/problems/missing-number/solution/)

```java
public int missingNumber(int[] nums) {
    int missing = nums.length;
    for (int i = 0; i < nums.length; i++) {
        missing ^= i ^ nums[i];
    }
    return missing;
}
```

### Bitwise AND of Numbers Range

题号：201

思路：

- 数组的数字是连续的，那么m,n范围内的二进制表示的末尾相同位置一定会出现不同的0,1.我们只要找出m,n的做左边起的最长相同的二进制头部即可

```java
public int rangeBitwiseAnd(int m, int n) {
    int count = 0;
    while (m != n) {
        m >>= 1;
        n >>= 1;
        count++;
    }
    return m <<= count;
}
```

### 问题：3Sum

题号：15

思路：

- 选择当前元素为target
- 以target后一元素为left，数组最后一个元素为right
- 移动left，right 满足条件保存下来
- 满足条件的相同元素跳过

```java
public List<List<Integer>> threeSum(int[] nums) {
    if (nums.length == 0) return new ArrayList<>();

    Arrays.sort(nums);
    List<List<Integer>> ans = new ArrayList<>();
    int len = nums.length;
    for (int i = 0; i < len - 2; i++) {
        if (nums[i] > 0) break;
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int target = nums[i];
        int left = i + 1;
        int right = nums.length - 1;
        while (left < right) {
            if (nums[left] + nums[right] + target == 0) {
                List<Integer> tmp = new ArrayList<>();
                tmp.add(target);
                tmp.add(nums[left]);
                tmp.add(nums[right]);
                ans.add(tmp);
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (right > left && nums[right] == nums[right - 1]) right--;
                left++;
                right--;
            }
            else if (nums[left] + nums[right] + target < 0) {
                left++;
            } else {
                right--;
            }
        }
    }
    return ans;
}
```

### 问题：4Sum

题号：18

```java
public List<List<Integer>> fourSum(int[] nums, int target) {
    if (nums.length == 0) return new ArrayList<>();
    Arrays.sort(nums);
    List<List<Integer>> ans = new ArrayList<>();
    for (int i = 0; i < nums.length - 3; i++) {
        // 由于target可以是大于零的数，所有这里删掉 if(nums[i] > 0) break;
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        for (int j = i + 1; j < nums.length - 2; j++) {
            // 对于第二个元素也许要加判断是否重复
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;
            int left = j + 1;
            int right = nums.length - 1;
            while (left < right) {
                int sum = nums[left] + nums[right] + nums[i] + nums[j];
                if (sum == target) {
                    List<Integer> tmp = new ArrayList<>();
                    tmp.add(nums[i]); tmp.add(nums[j]); tmp.add(nums[left]); tmp.add(nums[right]);
                    ans.add(tmp);
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < target) {
                    left++;
                } else {
                    right--;
                }
            }
        }
    }
    return ans;
}
```

### 问题：3Sum Closest

题号：16

```java
public int threeSumClosest(int[] nums, int target) {
    if (nums.length == 0) return 0;
    Arrays.sort(nums);
    int sum = nums[0] + nums[1] + nums[2];
    int dist = Math.abs(sum - target);
    for (int i = 0; i < nums.length - 2; i++) {
        int left = i + 1;
        int right = nums.length - 1;
        while (left < right) {
            int cursum = nums[i] + nums[left] + nums[right];
            int curdist = Math.abs(cursum - target);
            if (curdist < dist) {
                sum = cursum;
                dist = curdist;
            }
            if (target < cursum) {
                right--;
            } else if (target > cursum) {
                left++;
            } else {
                return cursum;
            }
        }
    }
    return sum;
}
```

## **Topological Sort**

### Course Schedule

题号：207 210

自己的实现 拓扑排序

```java
public boolean canFinish(int numCourses, int[][] prerequisites) {
    int[] in = new int[numCourses];
    int[][] m = buildGraph(numCourses, prerequisites, in);
    Deque<Integer> deque = new ArrayDeque<>();
    int count = 0;

    for (int i = 0; i < in.length; i++) {
        if (in[i] == 0) {
            deque.offer(i);
            count++;
        }
    }

    if (deque.isEmpty()) {
        return false;
    }

    while (!deque.isEmpty()) {
        int v = deque.poll();
        for (int i = 0; i < m[0].length; i++) {
            if (m[v][i] == 1) {
                in[i]--;
                if (in[i] == 0) {
                    deque.offer(i);
                    count++;
                }
                m[v][i] = 0;
            }
        }
    }

    return count == numCourses;
}

private int[][] buildGraph(int n, int[][] graph, int[] in) {
    int[][] m = new int[n][n];
    for (int i = 0; i < graph.length; i++) {
        int start = graph[i][0];
        int end = graph[i][1];
        m[start][end] = 1;
        in[end]++;
    }
    return m;
}
```