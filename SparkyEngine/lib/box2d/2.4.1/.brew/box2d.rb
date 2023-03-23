class Box2d < Formula
  desc "2D physics engine for games"
  homepage "https://box2d.org"
  url "https://github.com/erincatto/box2d/archive/v2.4.1.tar.gz"
  sha256 "d6b4650ff897ee1ead27cf77a5933ea197cbeef6705638dd181adc2e816b23c2"
  license "MIT"
  head "https://github.com/erincatto/Box2D.git", branch: "main"

  depends_on "cmake" => :build
  depends_on "doctest" => :test

  def install
    args = %w[
      -DBOX2D_BUILD_UNIT_TESTS=OFF
      -DBOX2D_BUILD_TESTBED=OFF
      -DBOX2D_BUILD_EXAMPLES=OFF
    ]

    system "cmake", ".", *args, *std_cmake_args
    system "cmake", "--build", "."
    system "cmake", "--install", "."
    pkgshare.install "unit-test/hello_world.cpp"
  end

  test do
    system ENV.cxx, pkgshare/"hello_world.cpp",
                    "-I#{Formula["doctest"].opt_include}/doctest",
                    "-L#{lib}", "-lbox2d",
                    "-std=c++11", "-o", testpath/"test"
    assert_match "[doctest] Status: SUCCESS!", shell_output("./test")
  end
end
