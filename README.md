# Multi-Client Server with ThreadPool

## Overview

This project implements a multi-client server architecture using a ThreadPool in C++. The server handles all socket connections and processes client requests through jobs defined by `ClientJob`. Each job represents a task, which can either be sending or receiving messages. The ThreadPool executes these jobs concurrently, allowing for efficient handling of multiple clients.

## Features

- Multi-threaded handling of client connections.
- A ThreadPool that manages worker threads to execute jobs based on `ClientJob`.
- Support for sending and receiving messages between clients and the server.
- Broadcasting messages to all connected clients.

## Components

- **Server**: Manages all client socket connections, accepts incoming messages, and utilizes the ThreadPool to process jobs.
- **ThreadPool**: A pool of worker threads that executes tasks defined by `ClientJob`. It separates the job management from socket handling, making the design clean and reusable.
- **ClientJob**: Represents a job to be processed. It can include information about the client socket and the message to be sent or received.

## How It Works

1. The server listens for incoming client connections.
2. Upon accepting a connection, a `ClientJob` is created and added to the ThreadPool.
3. The ThreadPool processes the job, either sending a message to the client or receiving a message from the client.
4. If a message is received, the server broadcasts it to all connected clients.

## Usage

1. Compile the project using CMake.
2. Run the server executable.
3. Run one or more client executables to connect to the server.

## Requirements

- C++11 or higher
- CMake

---

# Máy chủ đa kết nối với ThreadPool

## Tổng quan

Dự án này triển khai kiến trúc máy chủ đa kết nối sử dụng ThreadPool trong C++. Máy chủ xử lý tất cả các kết nối socket và xử lý các yêu cầu của khách hàng thông qua các công việc được định nghĩa bởi `ClientJob`. Mỗi công việc đại diện cho một nhiệm vụ, có thể là gửi hoặc nhận tin nhắn. ThreadPool thực hiện các công việc này đồng thời, cho phép xử lý hiệu quả nhiều khách hàng.

## Tính năng

- Xử lý đa luồng cho các kết nối khách hàng.
- Một ThreadPool quản lý các luồng làm việc để thực hiện các công việc dựa trên `ClientJob`.
- Hỗ trợ gửi và nhận tin nhắn giữa khách hàng và máy chủ.
- Phát sóng tin nhắn đến tất cả các khách hàng kết nối.

## Các thành phần

- **Máy chủ**: Quản lý tất cả các kết nối socket của khách hàng, chấp nhận các tin nhắn đến và sử dụng ThreadPool để xử lý các công việc.
- **ThreadPool**: Một nhóm các luồng làm việc thực hiện các nhiệm vụ được định nghĩa bởi `ClientJob`. Nó tách biệt việc quản lý công việc khỏi việc xử lý socket, giúp thiết kế trở nên sạch sẽ và tái sử dụng được.
- **ClientJob**: Đại diện cho một công việc cần được xử lý. Nó có thể bao gồm thông tin về socket khách hàng và tin nhắn cần gửi hoặc nhận.

## Cách hoạt động

1. Máy chủ lắng nghe các kết nối khách hàng đến.
2. Khi chấp nhận một kết nối, một `ClientJob` được tạo và thêm vào ThreadPool.
3. ThreadPool xử lý công việc, có thể là gửi tin nhắn đến khách hàng hoặc nhận tin nhắn từ khách hàng.
4. Nếu một tin nhắn được nhận, máy chủ phát sóng nó đến tất cả các khách hàng kết nối.

## Sử dụng

1. Biên dịch dự án bằng CMake.
2. Chạy tệp thực thi máy chủ.
3. Chạy một hoặc nhiều tệp thực thi khách hàng để kết nối với máy chủ.

## Yêu cầu

- C++11 trở lên
- CMake
