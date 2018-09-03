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
    for (int i  =1; i < nums.length; i++) {
        if (nums[i] != nums[i - 1]) {
            nums[n] = nums[i];
            n++;
        }
    }
    return n;
}
```

```java
// 排序好的数组，每个数字只允许出现一次
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

### 题目：从未排序的整数数组中找出最小的正整数

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