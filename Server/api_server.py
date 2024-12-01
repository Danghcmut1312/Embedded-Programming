from flask import Flask, request, jsonify
import csv
import os

app = Flask(__name__)

CSV_FILE = "data.csv"

def init_csv():
    if not os.path.exists(CSV_FILE):
        with open(CSV_FILE, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(['value'])

@app.route('/api/data', methods=['GET'])
def receive_data():
    sensor_value = request.args.get('value', type=int)
    
    if sensor_value is not None:
        if 1000 <= value <= 9999:
            with open(CSV_FILE, mode='a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow([sensor_value])
            return jsonify({"message": "Dữ liệu đã được nhận và lưu thành công!", "received_data": {"value": value}}), 200
        else:
            return jsonify({"error": "Giá trị 'value' phải là số có 4 chữ số!"}), 400
    else:
        return jsonify({"error": "Giá trị 'value' không được cung cấp trong query string!"}), 400

if __name__ == '__main__':
    init_csv()
    app.run(host='0.0.0.0', port=5000)
