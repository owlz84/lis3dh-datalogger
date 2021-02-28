#include <utils.h>
#include <unity.h>


short short_array[3] = {-6, -2, 2};
unsigned short ushort_array[3] = {1, 4, 10};

void test_array_max(void) {
  TEST_ASSERT_EQUAL(2, array_max(short_array));
  TEST_ASSERT_EQUAL(10, array_max(ushort_array));
}

void test_array_min(void) {
  TEST_ASSERT_EQUAL(-6, array_min(short_array));
  TEST_ASSERT_EQUAL(1, array_min(ushort_array));
}

void test_array_sum(void) {
  TEST_ASSERT_EQUAL(-6, array_sum(short_array));
  TEST_ASSERT_EQUAL(15, array_sum(ushort_array));
}

void test_array_len(void) {
  TEST_ASSERT_EQUAL(3, array_len(short_array));
  TEST_ASSERT_EQUAL(3, array_len(ushort_array));
}

void test_array_mean(void) {
  TEST_ASSERT_EQUAL(-2, array_mean(short_array));
  TEST_ASSERT_EQUAL(5, array_mean(ushort_array));
}

void test_array_center(void) {
  short short_centered_arr[3] = {-4, 0, 4};
  array_center(short_array);
  TEST_ASSERT_EQUAL_INT16_ARRAY(short_centered_arr, short_array, 3);

  // works but not valid
  unsigned short ushort_centered_arr[3] = {-4, -1, 5};
  array_center(ushort_array);
  TEST_ASSERT_EQUAL_INT16_ARRAY(ushort_centered_arr, ushort_array, 3);
}

int main(int argc, char **argv) {

  UNITY_BEGIN();
  RUN_TEST(test_array_max);
  RUN_TEST(test_array_min);
  RUN_TEST(test_array_sum);
  RUN_TEST(test_array_len);
  RUN_TEST(test_array_mean);
  RUN_TEST(test_array_center);
  UNITY_END();

  return 0;
}