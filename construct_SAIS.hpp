#include <iostream> 
#include <vector>

using namespace std; 

void print_sa2(vector<int> v){
	cout << '[';
	for(uint64_t i = 0; i < v.size(); ++i)
		if(i<v.size()-1)
			cout << v[i] << ' ';
		else
			cout << v[i];
	cout  << ']' << endl;
}

// An implementation of the "Suffix Array Induced Sorting" algorihtm by Nong et al.
vector<int> construct(vector<int> text, int rec) {
    int n = text.size(); // The length of the text
    int sigma = max(256, n); // The size of the alphabet. Default: 256
    vector<int> ln(n, -1); // LN array used for reducing text
    vector<int> sa(n, -1); // Suffix array. Undefined position: -1

    int amount_lms = 0; // Amount of LMS positions (calculated in phase 0)

    vector<int> types(n, -1);
    vector<int> count(sigma, 0); // Amount of occurrences of each character
    vector<int> c(sigma+1, 0); // C array

    vector<int> bkt_l(sigma, -1);
    vector<int> bkt_s(sigma, -1);
    vector<int> bkt_tmp(sigma, -1);

    if (n == 1) {
        sa[0] = 0;
        return sa;
    }

    // Phase 0
    {
        phase0:

        bool was_s_type = true; // Position n-1 is LMS by definition
        ++count[0];
        for (int i = n-2; 0 <= i; i--) { // From right to left
            ++count[text[i]];
            if (text[i] < text[i+1]) { // L type found
                types[i] = 1;
                was_s_type = true;
            } else if (text[i] > text[i+1]) { // S type found
                types[i] = 0;
                if (was_s_type) {
                    ++amount_lms;
                    types[i+1] = 2; // Alter previous type to LMS
                }
                was_s_type = false;
            } else { // Same character found -> same type
                types[i] = types[i+1];
            }
        }
    }
    
    vector<int> p(amount_lms, -1); // P array used to store LMS positions
    vector<int> text_new(amount_lms, -1);
    vector<int> sa_new(amount_lms, -1);
    vector<int> res(amount_lms, -1);

    // Calculating the C array
    c[0] = 0;
    for (int i = 0, sum = 0; i < sigma; ++i) {
        sum += count[i];
        c[i+1] = sum;
    }
    
    // Prepare start positions of S bucket (the end of the c-bucket)
    for (int i = 0; i < sigma; ++i) bkt_s[i] = c[i+1] - 1; // -1, since array starts at 0

    // Prepare start positions of L bucket (the beginning of the c-bucket)
    for (int i = 0; i < sigma; ++i) bkt_l[i] = c[i]; // -1, since array starts at 0

    // Phase 1
    {
        phase1:

        // Step I
        {
            phase1stepI:

            // Backup S bucket
            for (int i = 0; i < sigma; ++i) bkt_tmp[i] = bkt_s[i];

            // Insert all LMS positions into the end of the right bucket and into the P array
            for (int i = 1, b, k, pi = 0; i < n; ++i) if(types[i] == 2) {
                b = text[i]; // bucket b
                k = bkt_tmp[b]; // index k
                sa[k] = i; // save index i into the current end of bucket b of SA
                p[pi] = i; // save index i into the P array
                ++pi;
                --bkt_tmp[b]; // move index by one to the left 
            }
        }
        
        // Step II
        {
            phase1stepII:

            // Backup L bucket
            for (int i = 0; i < sigma; ++i) bkt_tmp[i] = bkt_l[i];

            // Insert all L type suffixes into SA 
            for (int i = 0, j, k; i < n; ++i) if (sa[i] != -1) {
                j = sa[i] - 1; // Position to be checked
                if (j != -1 && types[j] == 0) { // j must not exceed the text array 
                    k = text[j]; // bucket k
                    sa[bkt_tmp[k]] = j;
                    ++bkt_tmp[k]; // move index by one to the right 
                }
            }
        }

        // Step III
        {
            phase1stepIII:

            // Backup S bucket
            for (int i = 0; i < sigma; ++i) bkt_tmp[i] = bkt_s[i];

            // Insert all S type suffixes into SA
            for (int i = n-1, k, j; 0 <= i; --i) if (sa[i] != -1) {
                j = sa[i] - 1;
                if (j != -1 && (types[j] == 1 || types[j] == 2)) {
                    k = text[j]; // bucket k
                    sa[bkt_tmp[k]] = j;
                    --bkt_tmp[k]; // move index by one to the left 
                } 
            }
        }

        // Step IV
        int k = -1;
        {
            phase1stepIV:

            k = 0;
            ln[n-1] = k;
            int prev = n-1;
            for (int i = 1, j; i < n; ++i) {
                j = sa[i];
                if (types[j] == 2) {
                    bool equal = true; // If the LMS substrings are equal
                    int a, b;
                    // Check both LMS substrings until characters either differ, or the end of one substring has been reached
                    for (a = j, b = prev; (a == j || types[a] != 2) && (b == prev || types[b] != 2); ++a, ++b) if (text[a] != text[b]) { 
                        equal = false;
                        break;
                    }
                    // Check the last character if both substrings are of distinct length
                    if (text[a] != text[b] || (types[a] == 2 && types[b] != 2) || (types[a] != 2 && types[b] == 2)) equal = false;
                    if (!equal) ++k; 
                    ln[j] = k;
                    prev = j;                   
                }
            }
            for (int i = 0; i < amount_lms; ++i) {
                text_new[i] = ln[p[i]];
            }
        }
        
        // Step V
        {
            phase1stepV:
            
            // All entries in text_neu are distinct
            if (k+1 == amount_lms) for (int i = 0; i < amount_lms; ++i) sa_new[text_new[i]] = i; // compute sa_new in the base case
            else sa_new = construct(text_new, rec+1); // recursion

            for (int i = 0; i < amount_lms; ++i) res[i] = p[sa_new[i]]; // Calculate the order of the LMS 
        }        
    }

    // Phase 2
    {
        phase2:

        // Reset SA array
        for (int i = 0; i < n; ++i) sa[i] = -1;

        // Step I
        {
            phase2stepI:

            // Backup S bucket
            for (int i = 0; i < sigma; ++i) bkt_tmp[i] = bkt_s[i];

            // Insert LMS positions
            for (int i = amount_lms-1, j, k; 0 <= i; --i) {
                j = res[i];
                k = text[j]; // bucket k
                sa[bkt_tmp[k]] = j;
                --bkt_tmp[k]; // move index by one to the left 
            }
        }

        // Step II
        {
            phase2stepII:

            // Backup L bucket
            for (int i = 0; i < sigma; ++i) bkt_tmp[i] = bkt_l[i];

            // Insert all L type suffixes into SA 
            for (int i = 0, j, k; i < n; ++i) if (sa[i] != -1) {
                j = sa[i] - 1; // Position to be checked
                if (j != -1 && types[j] == 0) { // j must not exceed the text array 
                    k = text[j]; // bucket k
                    sa[bkt_tmp[k]] = j;
                    ++bkt_tmp[k]; // move index by one to the right 
                }
            }
        }

        // Step III
        {
            phase2stepIII:

            // Backup S bucket
            for (int i = 0; i < sigma; ++i) bkt_tmp[i] = bkt_s[i];

            // Insert all S type suffixes into SA
            for (int i = n-1, k, j; 0 <= i; --i) if (sa[i] != -1) { // sa[i] != -1 müsste nicht geprüft werden
                j = sa[i] - 1;
                if (j != -1 && (types[j] == 1 || types[j] == 2)) {
                    k = text[j]; // bucket k
                    sa[bkt_tmp[k]] = j;
                    --bkt_tmp[k]; // move index by one to the left 
                } 
            }
        }
    }
    
    ret:
    // Return the sorted suffix array
    return sa;
} 
