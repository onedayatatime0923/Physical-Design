1. 學號：b04901136
2. 姓名：張家銘
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ >
5. 檔案壓縮方式: <zip -r b04901136-p2.zip pa2>
6. 各檔案說明：
	 pa1/tool/*             : evaluation 程式
	 pa1/include/*          : include library(此次作業為空)
	 pa1/input/*            : 測資
	 pa1/lib/*              : library
	 pa1/output/*           : output file
	 pa1/src/*              : 純文字主程式
     pa1/prog2_routing-1.pdf: 作業說明
	 pa1/eval.sh            : eval script
	 pa1/run.sh             : run script
     pa1/CMakeLists.txt     : CMakeLists
	 pa1/readme.txt         : readme
	 pa1/report.docx        : report

7. 編譯方式說明：        	
   主程式：pa2/src/
	 請創建build資料夾
     mkdir build
     cd build
   在build目錄下，鍵入cmake指令
     cmake ..
   在build目錄下，鍵入make指令
     make
   即可完成編譯，
	 ( Makefile原先下的 Optimize 指令為 -O3 )

8. 執行、使用方式說明：
   主程式：
   編譯完成後，在檔案目錄下會產生一個執行檔
     ./gr
   執行檔的命令格式為：
   <executable_file_name> <input file name> <output file name>
   例如：要對 input_0.dat 執行，並且寫入 output_0.dat
   則在命令提示下鍵入
   ./gr input_0.dat output_0.dat

9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
	 主程式執行時不會在 console 同時輸出結果。
       
