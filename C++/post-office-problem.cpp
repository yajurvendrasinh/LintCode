// Time:  O(k * n^2)
// Space: O(n^2)

class Solution {
public:
    /**
     * @param A an integer array
     * @param k an integer
     * @return an integer
     */
    int postOffice(vector<int>& A, int k) {
        const int n = A.size();
        if (A.empty() || k >= n) {
            return 0;
        }

        sort(A.begin(), A.end());  // Time:  O(nlogn)

        // Precompute cost.
        // Time:  O(n^2)
        // Space: O(n^2)
        vector<vector<int>> cost(A.size() + 1, vector<int>(A.size() + 1, 0));
        computeMinCost(A, &cost);

        // DP of sum.
        // Time:  O(k * n^2)
        // Space: O(n)
        // sum[i][j] denotes the smallest sum of
        // picking i post offices for the first j houses.
        vector<vector<int>> sum(2, vector<int>(A.size() + 1, INT_MAX));
        sum[0][0] = 0;
        for (int i = 1; i <= k; ++i) {
            for (int j = 0; j <= n; ++j) {
                // We can skip this line due to sum[i][j] <= sum[i - 2][j]
                // sum[i % 2][j] = INT_MAX;
                for (int r = 1; r <= j; ++r) {
                    if (sum[(i - 1) % 2][j - r] != INT_MAX) {
                        sum[i % 2][j] = min(sum[i % 2][j],
                                            sum[(i - 1) % 2][j - r] +
                                            cost[j - r + 1][j]);
                    }
                }
            }
        }
        return sum[k % 2][n];
    }

    void computeMinCost(const vector<int>& A, vector<vector<int>> *cost) {
        // Min cost of building a post office between house (i, j).
        // This post office must be in median position.
        const int n = A.size();
        vector<int> before_diff(n), after_diff(n);
        for (int i = 1; i < n; ++i) {
            // before_diff[i] = Sum(A[i] - A[k]) for k < i
            before_diff[i] = before_diff[i - 1] + i * (A[i] - A[i - 1]);
        }
        for (int i = n - 2; i >= 0; --i) {
            // after_diff[i] = Sum(A[k] - A[i]) for k > i
            after_diff[i] = after_diff[i + 1] + (n - 1 - i) * (A[i + 1] - A[i]);
        }
        
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                int mid = i + (j - i) / 2;
                int before_mid = before_diff[mid] -
                                 (before_diff[i] + i * (A[mid] - A[i]));
                int after_mid = after_diff[mid] -
                                (after_diff[j] + (n - 1 - j) * (A[j] - A[mid]));
                (*cost)[i + 1][j + 1] +=  before_mid + after_mid;
            }
        }
    }
};

// Time:  O(n^3)
// Space: O(n^2)
class Solution2 {
public:
    /**
     * @param A an integer array
     * @param k an integer
     * @return an integer
     */
    int postOffice(vector<int>& A, int k) {
        const int n = A.size();
        if (A.empty() || k >= n) {
            return 0;
        }

        sort(A.begin(), A.end());  // Time:  O(nlogn)

        // Precompute cost.
        // Time:  O(n^3)
        // Space: O(n^2)
        vector<vector<int>> cost(A.size() + 1, vector<int>(A.size() + 1, 0));
        computeMinCost(A, &cost);

        // DP of sum.
        // Time:  O(k * n^2)
        // Space: O(k * n)
        // sum[i][j] denotes the smallest sum of
        // picking i post offices for the first j houses.
        vector<vector<int>> sum(k + 1, vector<int>(A.size() + 1, INT_MAX));
        sum[0][0] = 0;
        for (int i = 1; i <= k; ++i) {
            for (int j = 0; j < n; ++j) {
                if (sum[i - 1][j] != INT_MAX) {
                    for (int r = 1; j + r <= n; ++r) {
                       sum[i][j + r] = min(sum[i][j + r],
                                           sum[i - 1][j] + cost[j + 1][j + r]);
                    }
                }
            }
        }
        return sum[k][n];
    }

    void computeMinCost(const vector<int>& A, vector<vector<int>> *cost) {
        // Min cost of building a post office between house (i, j).
        // This post office must be in median position.
        const int n = A.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                int mid = (i + j) / 2;
                for (int r = i; r <= mid; ++r) {
                    (*cost)[i + 1][j + 1] += A[mid] - A[r];
                }
                for (int r = mid + 1; r <= j; ++r) {
                    (*cost)[i + 1][j + 1] += A[r] - A[mid];
                }
            }
        }
    }
};
