#include "receipt.h"
#include "calculator.h"
#include "math.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
using namespace cv;
using namespace tesseract;
using namespace std;
RNG rng(12345);
struct by_area {
    bool operator()(vector<Point> const & a, vector<Point> const & b){
        return contourArea(a) > contourArea(b);
    }
};
vector<Point> approx_contour(vector<Point> const & contour){
    double peri = arcLength(contour, true);
    vector<Point> res;
    approxPolyDP(contour, res, 0.032 * peri, true);
    return res;
}
int cv_resize(const Mat & orig, Mat & resized, const double & ratio) {
    int width = int(orig.cols * ratio);
    int height = int(orig.rows * ratio);
    resize(orig, resized, Size(width, height), INTER_AREA);
    return 0;
}
vector<Point2f> contour_to_rect(vector<Point> contour, double ratio){
    vector<Point2f> rect{Point2f(0,0), Point2f(0,0), Point2f(0,0), Point2f(0,0)};
    vector<double> sums{0, 0, 0, 0};
    vector<double> diffs{0, 0, 0, 0};
    for (int i = 0; i < contour.size(); ++i){
        sums[i] = contour[i].x + contour[i].y;
        diffs[i] = contour[i].x - contour[i].y;
    }
    auto it_1 = max_element(sums.begin(), sums.end());
    auto it_2 = min_element(sums.begin(), sums.end());
    auto it_3 = max_element(diffs.begin(), diffs.end());
    auto it_4 = min_element(diffs.begin(), diffs.end());
    rect[0] = contour[distance(sums.begin(), it_2)] / ratio;
    rect[2] = contour[distance(sums.begin(), it_1)] / ratio;
    rect[1] = contour[distance(diffs.begin(), it_3)] / ratio;
    rect[3] = contour[distance(diffs.begin(), it_4)] / ratio;
    return rect;
}

auto wrap_perspective(Mat orig, vector<Point2f> rect){
    Point tl = rect[0];
    Point tr = rect[1];
    Point br = rect[2];
    Point bl = rect[3];
    cout << rect.size();
    double widthA = sqrt(pow(br.x - bl.x, 2) + pow(br.y - bl.y, 2));
    double widthB = sqrt(pow(tr.x - tl.x, 2) + pow(tr.y - tl.y, 2));
    double heightA = sqrt(pow(tr.x - br.x, 2) + pow(tr.y - br.y, 2));
    double heightB = sqrt(pow(tl.x - bl.x, 2) + pow(tl.y - bl.y, 2));
    float maxWidth = float(max(int(widthA), int(widthB)));
    float maxHeight = float(max(int(heightA), int(heightB)));
    vector<Point2f> dst{Point2f(0.0,0.0), Point2f(maxWidth - 1.0, 0.0), Point2f(maxWidth - 1.0, maxHeight - 1.0), Point2f(0.0, maxHeight - 1.0)};
    cout << dst << endl;

    Mat M = getPerspectiveTransform(rect, dst);
    Mat warped;
    warpPerspective(orig, warped,  M, Size(maxWidth, maxHeight));
    return warped;

}
int main() {
    unordered_map<string, float> people;
    people.emplace("Aaron", 0);
    people.emplace("Nathan", 0);
    people.emplace("Ricky", 0);
    Receipt receipt;
    Calculator calculator(people);
    string item_1 = "carrots";
    string item_2 = "potatoes";
    string item_3 = "cleaning";
    float price_1 = 5.00;
    float price_2 = 7.56;
    float price_3 = 6.68;
    set<string> people_1;
    set<string> people_2;
    people_1.emplace("Aaron");
    people_1.emplace("Nathan");
    people_2.emplace("Ricky");
    people_2.emplace("Nathan");
    //So basically, can have multiple lines of same item, but everyone needs to share all those items evenly
    receipt.set_item(item_1, people_1, price_1);
    receipt.set_item(item_2, people_1, price_2);
    receipt.set_item(item_1, people_1, price_1);
    receipt.set_item(item_3, people_2, price_3);
    //receipt.set_item(item_1, people_2, price_1);
    calculator.parse_receipt(receipt);
    calculator.print_partitions();
    calculator.calculate_prices();
    calculator.print_prices();

    string IMAGE_PATH= "./tjs.jpg";
    Mat original = imread(IMAGE_PATH);
    Mat resized_down;
    Mat gray;
    Mat blurred;
    Mat dilated;
    Mat edged;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    double ratio = 750.0 / original.rows;
    cout << ratio << endl;
    cv_resize(original, resized_down, ratio);
    cvtColor(resized_down, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    auto rectKernel = getStructuringElement(MORPH_RECT, Size(9,9));
    dilate(blurred, dilated, rectKernel);
    Canny(dilated, edged, 150, 250);
    findContours(edged, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    Mat img_w_contours;
    Mat orig_copy;
    resized_down.copyTo(orig_copy);
    sort(contours.begin(), contours.end(), by_area());
    vector<vector<Point>> largest_contour;
    largest_contour.push_back(contours[0]);
    vector<vector<Point>> rect_contour;
    rect_contour.push_back(approx_contour(largest_contour[0]));
    drawContours(orig_copy, rect_contour,  -1, (0, 255, 255), 2);
    for (auto & r : rect_contour) {
        cout << r << endl;
    }
    Mat another_copy;
    original.copyTo(another_copy);

    Mat scanned = wrap_perspective(another_copy, contour_to_rect(rect_contour[0], ratio));
    Mat dst;
    Mat thresholded;
    cvtColor(scanned, dst, COLOR_BGR2GRAY);
    threshold(dst, thresholded, 160, 255, THRESH_BINARY);
    imwrite("res.jpg", thresholded);

    auto* api = new TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    Pix *image = pixRead("./res.jpg");
    api->SetImage(image);
    cout << api->GetUTF8Text() << endl;

    api->End();
    delete api;
    pixDestroy(&image);
    int k = waitKey();


    return 0;
}
