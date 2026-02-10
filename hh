<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GoVn.task | Physical Interface Protocol</title>
    
    <!-- Frameworks & Fonts -->
    <script src="https://cdn.tailwindcss.com"></script>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700;800&family=Space+Grotesk:wght@300;500;700&family=Syne:wght@700;800&family=JetBrains+Mono&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">

    <script>
        tailwind.config = {
            theme: {
                extend: {
                    fontFamily: {
                        sans: ['Inter', 'Space Grotesk', 'sans-serif'],
                        display: ['Syne', 'Inter', 'sans-serif'],
                        mono: ['JetBrains Mono', 'monospace'],
                    },
                    colors: {
                        porcelain: '#F8F9FA',
                        electric: '#0052FF',
                        magenta: '#FF00A8',
                        cyber: '#00F0FF',
                        coffee: '#3E2723',
                        hatBeige: '#FDFCF0',
                        trainBlue: '#0066B3',
                        trainRed: '#D71920',
                        chairRed: '#EE2B2B',
                        chairRedDark: '#B31D1D',
                    }
                }
            }
        }
    </script>

    <style>
        :root { --bg-color: #F8F9FA; --accent: #0052FF; }
        html { scroll-behavior: smooth; }
        body {
            background-color: var(--bg-color);
            color: #1A1A1A;
            cursor: none;
            overflow-x: hidden;
            user-select: none;
            touch-action: none;
            font-family: 'Inter', 'Space Grotesk', sans-serif;
        }

        #cursor-container {
            position: fixed; top: 0; left: 0;
            width: 100%; height: 100%;
            pointer-events: none; z-index: 9999;
        }

        #custom-cursor {
            position: absolute;
            width: 80px; height: 120px;
            will-change: transform;
            filter: drop-shadow(0 8px 12px rgba(0,0,0,0.15));
            /* Giảm transition để con trỏ bám sát chuột hơn */
            transition: transform 0.02s linear;
            transform-origin: 50% 5px; 
        }

        #custom-cursor.grabbing {
            filter: drop-shadow(0 4px 6px rgba(0,52,255,0.4));
            transform: scale(0.9);
        }

        /* Train Physics Styles */
        .train-rail-container {
            position: relative;
            margin-top: 50px;
            width: 100%; height: 120px;
            z-index: 40;
            pointer-events: none;
        }

        .car-physics-node {
            position: absolute;
            will-change: transform;
            cursor: grab;
            transform-origin: center center;
            pointer-events: auto;
        }
        .car-physics-node:active { cursor: grabbing; }

        .wheel {
            position: absolute; bottom: -6px;
            width: 14px; height: 14px;
            background: #1A1A1A; border: 2px solid #333;
            border-radius: 50%;
        }

        .locomotive-d19e {
            width: 110px; height: 50px;
            background: #0066B3; border: 2px solid #000;
            position: relative; z-index: 15;
            border-radius: 4px 12px 4px 4px;
        }

        .locomotive-d19e::after, .carriage-d19e::after {
            content: ''; position: absolute; bottom: 0; left: -2px;
            width: calc(100% + 4px); height: 14px;
            background: #D71920; border-top: 2px solid #000;
        }

        .vn-flag-container {
            position: absolute;
            top: -22px;
            left: 10px;
            transform: rotate(-15deg);
            pointer-events: none;
        }
        .flag-pole { width: 2px; height: 25px; background: #333; }
        .flag-cloth {
            position: absolute; top: 0; left: 2px;
            width: 18px; height: 12px;
            background: #da251d;
            display: flex; align-items: center; justify-content: center;
            animation: flagWave 2s infinite ease-in-out;
            transform-origin: left center;
        }
        .flag-star { color: #ffff00; font-size: 8px; }

        @keyframes flagWave {
            0%, 100% { transform: scaleX(1) skewY(0deg); }
            50% { transform: scaleX(0.9) skewY(5deg); }
        }

        .carriage-d19e {
            width: 95px; height: 46px;
            background: #0066B3; border: 2px solid #000;
            position: relative;
            display: flex; justify-content: space-around; padding-top: 6px;
            border-radius: 4px;
        }

        .coupler { position: absolute; left: -10px; bottom: 8px; width: 12px; height: 4px; background: #1A1A1A; }
        .car-window { width: 18px; height: 12px; background: #CCF7FF; border: 1.5px solid #000; border-radius: 2px; }

        .ai-panel {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(20px);
            border: 2px solid #000;
            box-shadow: 12px 12px 0px rgba(0, 0, 0, 1);
        }

        .pixel-border {
            border: 2px solid #000;
            box-shadow: 4px 4px 0px #000;
            transition: all 0.2s;
        }
        .pixel-border:hover { transform: translate(-1px, -1px); box-shadow: 6px 6px 0px #000; }
        .pixel-border:active { transform: translate(1px, 1px); box-shadow: 1px 1px 0px #000; }

        .loc-btn.active {
            background: #0052FF;
            color: white;
            border-color: #0052FF;
            box-shadow: 4px 4px 0px #000;
        }
        
        a, button, input { cursor: none !important; }
        .lang-active { color: #0052FF; font-weight: 800; }

        .finding-icon { animation: scan 2s infinite ease-in-out; }
        @keyframes scan {
            0% { transform: translate(-20px, -20px); }
            50% { transform: translate(20px, 20px); }
            100% { transform: translate(-20px, -20px); }
        }

        .floating-asset {
            position: absolute;
            pointer-events: auto;
            z-index: 40;
            transition: transform 0.3s cubic-bezier(0.175, 0.885, 0.32, 1.275);
        }

        @keyframes assetFloat {
            0%, 100% { transform: translateY(0) rotate(0deg); }
            50% { transform: translateY(-8px) rotate(1deg); }
        }
        .coffee-container { animation: assetFloat 4s infinite ease-in-out; }
        .chair-container { animation: assetFloat 5s infinite ease-in-out -1s; }

        @keyframes coffeeDrip {
            0% { transform: translateY(0) scale(1); opacity: 0; }
            5% { opacity: 1; }
            85% { transform: translateY(18px) scale(0.9); opacity: 1; }
            95%, 100% { transform: translateY(22px) scale(1.5); opacity: 0; }
        }
        .drip-drop {
            transform-origin: center top;
            animation: coffeeDrip 3s infinite cubic-bezier(0.5, 0, 0.8, 0.5);
        }

        .broker-footer-cta {
            width: 100%;
            padding: 40px 0 60px 0;
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 16px;
            z-index: 50;
        }

        .broker-glow {
            box-shadow: 0 0 20px rgba(0, 82, 255, 0.2);
            transition: all 0.3s ease;
        }
        .broker-glow:hover {
            box-shadow: 0 0 35px rgba(255, 0, 168, 0.4);
            background: #000;
            color: #fff;
            transform: translateY(-4px);
        }
    </style>
</head>
<body class="antialiased">

    <div id="cursor-container">
        <div id="custom-cursor">
            <svg viewBox="0 0 100 130" fill="none" xmlns="http://www.w3.org/2000/svg">
                <path id="hat-strap" d="M30 65 Q 50 110 70 65" stroke="#D71920" stroke-width="2.5" stroke-linecap="round" fill="none" />
                <ellipse cx="50" cy="70" rx="35" ry="5" fill="black" opacity="0.1"/>
                <path d="M50 5L90 65H10L50 5Z" fill="#FDFCF0" stroke="black" stroke-width="1.5"/>
                <circle cx="50" cy="5" r="3" fill="#00F0FF" class="animate-pulse shadow-lg"/>
            </svg>
        </div>
    </div>

    <!-- Navigation -->
    <nav class="fixed top-0 w-full z-50 px-6 py-6">
        <div class="max-w-7xl mx-auto flex justify-between items-center">
            <div class="flex items-center gap-2">
                <div class="w-10 h-10 bg-electric flex items-center justify-center rounded-full">
                    <span class="text-white font-display font-black text-xl">G</span>
                </div>
                <span class="font-display font-bold text-xl tracking-tighter uppercase">GoVn.task</span>
            </div>
            <div class="hidden md:flex gap-8 items-center bg-white/90 px-8 py-3 rounded-full border-2 border-black shadow-lg backdrop-blur-md">
                <button onclick="scrollToId('interface')" data-i18n="nav-protocol" class="text-[11px] font-bold uppercase tracking-widest hover:text-electric transition-colors">Giao Thức</button>
                <button onclick="scrollToId('interface')" data-i18n="nav-tasks" class="text-[11px] font-bold uppercase tracking-widest hover:text-electric transition-colors">Nhiệm Vụ</button>
            </div>
            <div class="flex items-center gap-4">
                <div class="flex items-center bg-black rounded-full p-1 border-2 border-black shadow-xl">
                    <button class="text-white px-5 py-2 font-bold text-[10px] uppercase tracking-widest hover:text-electric transition-colors border-r border-white/20" data-i18n="nav-login">Đăng Nhập</button>
                    <button class="text-white px-5 py-2 font-bold text-[10px] uppercase tracking-widest hover:text-electric transition-colors" data-i18n="nav-signup">Đăng Ký</button>
                </div>
            </div>
        </div>
    </nav>

    <!-- Main Content -->
    <section class="min-h-screen flex flex-col items-center relative overflow-hidden px-6 pt-32">
        <div class="max-w-7xl mx-auto w-full grid lg:grid-cols-12 gap-12 items-center relative z-10">
            
            <div class="lg:col-span-6">
                <div class="mb-6 flex items-center gap-3">
                    <span data-i18n="tag-active" class="px-3 py-1 bg-magenta text-white text-[10px] font-black uppercase tracking-widest">Kích Hoạt Node</span>
                    <span class="font-mono text-[10px] text-slate-500 uppercase tracking-tighter">CAL_INTAKE: 1900 KCAL</span>
                </div>
                <h1 class="font-display text-[55px] lg:text-[75px] font-black leading-[0.85] tracking-tighter mb-8 text-black uppercase">
                    Trans-Vietnam<br><span class="text-electric">Heritage</span>
                </h1>
                <p data-i18n="hero-desc" class="max-w-md font-sans text-lg text-slate-500 leading-relaxed mb-10">
                    Hệ thống vận tải di sản số hóa. Nhấn giữ đoàn tàu để di chuyển khối dữ liệu vật lý qua các miền.
                </p>
            </div>

            <!-- AI Task Broker Panel -->
            <div id="interface" class="lg:col-span-6 ai-panel p-8 rounded-xl relative z-30 overflow-visible scroll-mt-24">
                
                <!-- COFFEE PHIN ASSET -->
                <div class="absolute -top-24 -left-16 w-32 h-48 coffee-container hover:scale-105 floating-asset">
                    <svg viewBox="0 0 100 150" fill="none" xmlns="http://www.w3.org/2000/svg">
                        <path d="M30 90 L70 90 L65 135 Q 64 142 50 142 Q 36 142 35 135 L30 90 Z" fill="white" fill-opacity="0.2" stroke="#000" stroke-width="2"/>
                        <path d="M34 125 L66 125 L65 135 Q 64 141 50 141 Q 36 141 35 135 L34 125 Z" fill="#FFFBEB"/>
                        <path d="M31 100 L69 100 L66 125 L34 125 L31 100 Z" fill="#3E2723"/>
                        <rect x="25" y="82" width="50" height="4" rx="2" fill="#A1A1A1" stroke="#000" stroke-width="1.5"/>
                        <path d="M35 55 L65 55 L62 82 L38 82 Z" fill="#D1D1D1" stroke="#000" stroke-width="1.5"/>
                        <path d="M35 55 Q 50 48 65 55" stroke="#000" stroke-width="1.5"/>
                        <circle cx="50" cy="50" r="3" fill="#A1A1A1" stroke="#000" stroke-width="1.2"/>
                        <defs>
                            <clipPath id="drip-clip">
                                <rect x="0" y="0" width="100" height="100" />
                            </clipPath>
                        </defs>
                        <circle cx="50" cy="85" r="2.5" fill="#3E2723" class="drip-drop" clip-path="url(#drip-clip)"/>
                    </svg>
                </div>
                
                <!-- RED PLASTIC STOOL - SOFT EDGES -->
                <div class="absolute -bottom-16 -right-20 w-48 h-48 chair-container hover:scale-105 floating-asset">
                     <svg viewBox="0 0 100 100" fill="none" xmlns="http://www.w3.org/2000/svg">
                        <ellipse cx="50" cy="92" rx="35" ry="4" fill="black" fill-opacity="0.1"/>
                        <path d="M32 40 L28 82 Q 28 85 31 85 L32 85 Q 35 85 35 82 L38 40 Z" fill="#991B1B" stroke="#000" stroke-width="1.5"/>
                        <path d="M68 40 L72 82 Q 72 85 69 85 L68 85 Q 65 85 65 82 L62 40 Z" fill="#991B1B" stroke="#000" stroke-width="1.5"/>
                        <path d="M25 40 L18 87 Q 18 90 21 90 L25 90 Q 28 90 28 87 L32 40 Z" fill="#EE2B2B" stroke="#000" stroke-width="2"/>
                        <path d="M75 40 L82 87 Q 82 90 79 90 L75 90 Q 72 90 72 87 L68 40 Z" fill="#EE2B2B" stroke="#000" stroke-width="2"/>
                        <path d="M22 40 Q 20 40 20 42 L22 53 Q 22.5 55 25 55 L75 55 Q 77.5 55 78 53 L80 42 Q 80 40 78 40 Z" fill="#EE2B2B" stroke="#000" stroke-width="2"/>
                        <path d="M33 20 Q 30 20 28 22 L20 38 Q 19 40 22 40 L78 40 Q 81 40 80 38 L72 22 Q 70 20 67 20 Z" fill="#FF4D4D" stroke="#000" stroke-width="2"/>
                        <rect x="42" y="27" width="16" height="5" rx="3" fill="#B31D1D" stroke="#000" stroke-width="1.2"/>
                        <path d="M35 42 L35 52 M50 42 L50 52 M65 42 L65 52" stroke="#000" stroke-width="1" stroke-linecap="round" opacity="0.3"/>
                     </svg>
                </div>

                <div class="flex justify-between items-center mb-6">
                    <div class="flex flex-col">
                        <h2 data-i18n="panel-title" class="font-display text-2xl font-black flex items-center gap-2 tracking-tight">Giao thức AI Broker ✨</h2>
                        <span data-i18n="panel-subtitle" class="text-[8px] font-mono text-slate-400 uppercase tracking-wider">KẾT NỐI KHÁCH HÀNG VỚI BROKER</span>
                    </div>
                    <span id="status-tag" class="px-2 py-0.5 bg-green-100 text-green-600 font-mono text-[9px] font-bold rounded">TRỰC TUYẾN</span>
                </div>
                
                <div class="space-y-8">
                    <!-- Location Selection -->
                    <div>
                        <label data-i18n="label-loc" class="block text-[10px] font-black uppercase mb-3 tracking-widest text-slate-400">01. Chọn Location Node</label>
                        <div class="grid grid-cols-3 gap-3">
                            <button onclick="selectLoc('Hà Nội', this)" class="loc-btn pixel-border py-3 font-bold text-[12px] uppercase bg-white">Hà Nội</button>
                            <button onclick="selectLoc('Đà Nẵng', this)" class="loc-btn pixel-border py-3 font-bold text-[12px] uppercase bg-white">Đà Nẵng</button>
                            <button onclick="selectLoc('Sài Gòn', this)" class="loc-btn pixel-border py-3 font-bold text-[12px] uppercase bg-white active">Sài Gòn</button>
                        </div>
                    </div>

                    <!-- Task Input -->
                    <div>
                        <label data-i18n="label-task" class="block text-[10px] font-black uppercase mb-3 tracking-widest text-slate-400">02. Định nghĩa nhiệm vụ</label>
                        <div class="flex gap-2">
                            <input type="text" id="ai-input" data-i18n-placeholder="input-placeholder" placeholder="Nhập hành động (ví dụ: Chụp ảnh cafe, Mua quà...)" 
                                class="flex-1 bg-slate-50 border-2 border-slate-200 p-4 font-mono text-sm focus:outline-none focus:border-electric transition-all text-black cursor-text">
                            <button onclick="processAI()" data-i18n="btn-deploy" class="bg-black text-white px-6 font-bold text-[12px] uppercase hover:bg-electric transition-colors">
                                Triển Khai
                            </button>
                        </div>
                    </div>

                    <!-- States -->
                    <div id="ai-idle" class="py-8 text-center border-2 border-dashed border-slate-200 rounded-lg">
                        <p data-i18n="idle-msg" class="text-[10px] text-slate-400 font-mono uppercase tracking-widest">Hệ thống đang chờ lệnh</p>
                    </div>

                    <div id="ai-finding" class="hidden py-8 text-center border-2 border-slate-100 rounded-lg bg-slate-50">
                        <div class="finding-icon text-electric text-3xl mb-3">
                            <i class="fas fa-microchip"></i>
                        </div>
                        <p data-i18n="finding-msg" class="text-[10px] font-mono animate-pulse uppercase">AI ĐANG PHÂN TÍCH & KẾT NỐI BROKER...</p>
                    </div>

                    <div id="ai-result" class="hidden space-y-4 animate-[fadeIn_0.5s]">
                        <div class="bg-white border-2 border-black p-4 flex gap-4 items-center shadow-lg relative overflow-hidden group">
                            <div class="absolute -right-4 -top-4 w-12 h-12 bg-electric rotate-45 group-hover:bg-magenta transition-colors"></div>
                            <div class="w-16 h-16 bg-slate-200 border-2 border-black overflow-hidden flex-shrink-0">
                                <img id="broker-img" src="" alt="Broker Avatar" class="w-full h-full object-cover">
                            </div>
                            <div class="flex-1">
                                <div class="flex justify-between items-start">
                                    <div>
                                        <p data-i18n="broker-found-tag" class="text-[8px] font-mono text-slate-400 uppercase tracking-wider">Đã tìm thấy Broker</p>
                                        <h3 id="broker-name" class="font-display font-black text-lg leading-none mt-1">---</h3>
                                        <div class="flex gap-2 mt-1">
                                            <span data-i18n="tag-verified" class="text-[8px] font-bold bg-green-100 text-green-700 px-1 border border-green-200">XÁC MINH</span>
                                            <span class="text-[8px] font-bold bg-slate-100 text-slate-700 px-1 border border-slate-200">⭐ 4.9</span>
                                        </div>
                                    </div>
                                </div>
                                <div class="mt-2 text-[9px] font-mono text-slate-500 border-t border-slate-100 pt-1">
                                    <span data-i18n="est-cost">Giá dự kiến</span>: <span id="res-price" class="text-electric font-bold text-sm">$0.00</span> • <span data-i18n="est-time">TG</span>: <span id="res-time" class="font-bold">--</span>
                                </div>
                            </div>
                            <button onclick="openPayment()" class="h-full px-4 bg-black text-white font-black text-[10px] uppercase hover:bg-electric transition-colors flex flex-col justify-center items-center gap-1">
                                <span data-i18n="btn-book">ĐẶT</span>
                                <i class="fas fa-arrow-right"></i>
                            </button>
                        </div>
                        
                        <div class="bg-slate-50 p-3 border-l-4 border-electric text-[10px] font-mono text-slate-600 leading-relaxed italic">
                            "<span id="res-reasoning">...</span>"
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Train Container -->
        <div class="train-rail-container">
            <div id="car-0" class="car-physics-node">
                <div class="locomotive-d19e shadow-xl">
                    <div class="vn-flag-container"><div class="flag-pole"></div><div class="flag-cloth"><i class="fas fa-star flag-star"></i></div></div>
                    <div class="absolute top-[4px] right-[8px] w-[35px] h-[12px] bg-[#CCF7FF] border-[1.5px] border-black"></div>
                    <div class="absolute top-[20px] w-full h-[6px] bg-white border-y border-black"></div>
                    <div class="absolute top-[22px] left-1/2 -translate-x-1/2 font-mono text-[5px] font-black text-trainBlue uppercase">ĐỔI MỚI</div>
                    <div class="wheel left-[10px]"></div><div class="wheel left-[30px]"></div>
                    <div class="wheel right-[10px]"></div><div class="wheel right-[30px]"></div>
                </div>
            </div>
            <div id="car-1" class="car-physics-node"><div class="carriage-d19e shadow-lg"><div class="coupler"></div><div class="car-window"></div><div class="car-window"></div><div class="car-window"></div><div class="wheel left-[15px]"></div><div class="wheel right-[15px]"></div></div></div>
            <div id="car-2" class="car-physics-node"><div class="carriage-d19e shadow-lg"><div class="coupler"></div><div class="car-window"></div><div class="car-window"></div><div class="car-window"></div><div class="wheel left-[15px]"></div><div class="wheel right-[15px]"></div></div></div>
            <div id="car-3" class="car-physics-node"><div class="carriage-d19e shadow-lg"><div class="coupler"></div><div class="car-window"></div><div class="car-window"></div><div class="car-window"></div><div class="wheel left-[15px]"></div><div class="wheel right-[15px]"></div></div></div>
            
            <div id="main-rail" class="absolute bottom-[2px] w-full h-[6px] bg-slate-800 rounded-full border-t border-black"></div>
            <div class="absolute bottom-[-10px] w-full flex justify-around opacity-30">
                <script>for(let i=0; i<35; i++) document.write('<div class="w-[6px] h-4 bg-black rounded-sm"></div>')</script>
            </div>
        </div>

        <footer class="broker-footer-cta">
            <div class="flex flex-col items-center gap-2 group">
                <span class="text-[10px] font-mono text-slate-400 uppercase tracking-[0.3em] opacity-80 group-hover:opacity-100 transition-opacity">If you are in Vietnam</span>
                <button class="broker-glow bg-white border-2 border-black px-12 py-4 font-display font-black text-base uppercase tracking-tight active:scale-95 transition-all">
                    Become a GoBroker <i class="fas fa-bolt ml-3 text-electric group-hover:animate-pulse"></i>
                </button>
            </div>
            <p class="mt-8 text-[8px] font-mono text-slate-300 uppercase tracking-widest">GoVn.task Node Protocol v2.5.2</p>
        </footer>
    </section>

    <!-- Payment Modal -->
    <div id="payment-modal" class="fixed inset-0 z-[100] bg-black/80 backdrop-blur-md hidden flex items-center justify-center p-6">
        <div class="bg-white pixel-border w-full max-w-sm p-8 relative animate-[fadeIn_0.3s]">
            <button onclick="closePayment()" class="absolute top-4 right-4 text-slate-400 hover:text-black">
                <i class="fas fa-times"></i>
            </button>
            <h2 data-i18n="pay-title" class="font-display text-2xl font-black uppercase mb-1 tracking-tight">Thanh Toán</h2>
            <p data-i18n="pay-subtitle" class="text-[10px] font-mono text-slate-400 mb-6 uppercase">Cổng thanh toán an toàn</p>
            
            <div class="mb-6 p-4 bg-porcelain border-2 border-black flex justify-between items-center">
                <span data-i18n="pay-total-label" class="text-[10px] font-bold uppercase text-slate-500">Tổng cộng</span>
                <span id="pay-total" class="text-3xl font-display font-black text-electric">$0.00</span>
            </div>
            
            <div class="space-y-3">
                <button onclick="confirmPay('Credit Card')" class="w-full pixel-border p-4 font-bold text-xs uppercase hover:bg-slate-50 flex justify-between items-center group transition-all">
                    <div class="flex items-center gap-3">
                        <div class="w-8 h-5 bg-black rounded-sm"></div>
                        <span>Credit Card</span>
                    </div>
                    <i class="fas fa-chevron-right text-slate-300 group-hover:text-black"></i>
                </button>
                <button onclick="confirmPay('Crypto')" class="w-full pixel-border p-4 font-bold text-xs uppercase hover:bg-slate-50 flex justify-between items-center group transition-all">
                    <div class="flex items-center gap-3">
                        <i class="fab fa-bitcoin text-xl text-orange-500"></i>
                        <span>Crypto (USDC/SOL)</span>
                    </div>
                    <i class="fas fa-chevron-right text-slate-300 group-hover:text-black"></i>
                </button>
            </div>
            <p data-i18n="pay-escrow" class="mt-6 text-center text-[8px] font-mono text-slate-400 uppercase italic">Cung cấp bởi GoVn.task Escrow</p>
        </div>
    </div>

    <!-- Language Selector -->
    <div class="fixed bottom-8 left-8 z-[100] bg-white border-2 border-black p-2 flex gap-4 font-mono text-[10px] shadow-[8px_8px_0_#000]">
        <button id="lang-en" onclick="changeLang('EN')" class="hover:text-electric px-2">EN</button>
        <div class="w-[1px] bg-black/10"></div>
        <button id="lang-vn" onclick="changeLang('VN')" class="lang-active px-2 font-black">VN</button>
    </div>

    <script>
        const translations = {
            'EN': {
                'nav-protocol': 'Protocol', 'nav-tasks': 'Tasks', 'nav-login': 'Log In', 'nav-signup': 'Sign Up',
                'tag-active': 'Node Active',
                'hero-desc': 'Digital heritage logistics system. Hold and drag the train to move physical data blocks.',
                'panel-title': 'AI Broker Protocol ✨', 'panel-subtitle': 'CONNECTING CUSTOMERS TO BROKERS',
                'label-loc': '01. Select Location Node', 'label-task': '02. Define Task',
                'input-placeholder': 'Enter action (e.g. Photo of coffee, Buy gifts...)', 'btn-deploy': 'Deploy',
                'idle-msg': 'System waiting for command', 'finding-msg': 'AI ANALYZING & CONNECTING BROKER...',
                'broker-found-tag': 'Broker Found', 'tag-verified': 'VERIFIED',
                'est-cost': 'Est. Cost', 'est-time': 'Time', 'btn-book': 'BOOK',
                'pay-title': 'Payment', 'pay-subtitle': 'Secure Payment Gateway',
                'pay-total-label': 'Total', 'pay-escrow': 'Powered by GoVn.task Escrow',
                'alert-success': (price, method, broker) => `Payment ${price} successful via ${method}!\nBroker ${broker} has accepted the task.`,
                'res-reasoning': (val, loc) => `AI analyzed the request "${val}" and optimized the route for the Broker at ${loc}.`
            },
            'VN': {
                'nav-protocol': 'Giao Thức', 'nav-tasks': 'Nhiệm Vụ', 'nav-login': 'Đăng Nhập', 'nav-signup': 'Đăng Ký',
                'tag-active': 'Kích Hoạt Node',
                'hero-desc': 'Hệ thống vận tải di sản số hóa. Nhấn giữ đoàn tàu để di chuyển khối dữ liệu vật lý qua các miền.',
                'panel-title': 'Giao thức AI Broker ✨', 'panel-subtitle': 'KẾT NỐI KHÁCH HÀNG VỚI BROKER',
                'label-loc': '01. Chọn Location Node', 'label-task': '02. Định nghĩa nhiệm vụ',
                'input-placeholder': 'Nhập hành động (ví dụ: Chụp ảnh cafe, Mua quà...)', 'btn-deploy': 'Triển Khai',
                'idle-msg': 'Hệ thống đang chờ lệnh', 'finding-msg': 'AI ĐANG PHÂN TÍCH & KẾT NỐI BROKER...',
                'broker-found-tag': 'Đã tìm thấy Broker', 'tag-verified': 'XÁC MINH',
                'est-cost': 'Giá dự kiến', 'est-time': 'TG', 'btn-book': 'ĐẶT',
                'pay-title': 'Thanh Toán', 'pay-subtitle': 'Cổng thanh toán an toàn',
                'pay-total-label': 'Tổng cộng', 'pay-escrow': 'Cung cấp bởi GoVn.task Escrow',
                'alert-success': (price, method, broker) => `Thanh toán ${price} thành công qua ${method}!\nBroker ${broker} đã nhận nhiệm vụ.`,
                'res-reasoning': (val, loc) => `AI đã phân tích yêu cầu "${val}" và tối ưu hóa lộ trình cho Broker tại ${loc}.`
            }
        };

        const brokers = [
            { name: "Minh Quân", img: "https://api.dicebear.com/7.x/avataaars/svg?seed=Quan", city: "Sài Gòn" },
            { name: "Thanh Hằng", img: "https://api.dicebear.com/7.x/avataaars/svg?seed=Hang", city: "Hà Nội" },
            { name: "Anh Đức", img: "https://api.dicebear.com/7.x/avataaars/svg?seed=Duc", city: "Đà Nẵng" }
        ];

        let selectedLocation = 'Sài Gòn';
        let currentLang = 'VN';

        function selectLoc(loc, btn) {
            selectedLocation = loc;
            document.querySelectorAll('.loc-btn').forEach(b => b.classList.remove('active'));
            btn.classList.add('active');
        }

        function scrollToId(id) { 
            const element = document.getElementById(id);
            if (element) element.scrollIntoView({ behavior: 'smooth' }); 
        }

        function changeLang(lang) {
            currentLang = lang;
            const btnEn = document.getElementById('lang-en');
            const btnVn = document.getElementById('lang-vn');
            btnEn.classList.toggle('lang-active', lang === 'EN');
            btnEn.classList.toggle('font-black', lang === 'EN');
            btnVn.classList.toggle('lang-active', lang === 'VN');
            btnVn.classList.toggle('font-black', lang === 'VN');

            document.querySelectorAll('[data-i18n]').forEach(el => {
                const key = el.getAttribute('data-i18n');
                if (translations[lang][key]) el.innerText = translations[lang][key];
            });

            document.querySelectorAll('[data-i18n-placeholder]').forEach(el => {
                const key = el.getAttribute('data-i18n-placeholder');
                if (translations[lang][key]) el.placeholder = translations[lang][key];
            });
        }

        function processAI() {
            const val = document.getElementById('ai-input').value.trim();
            if(!val) return;
            const idle = document.getElementById('ai-idle'), finding = document.getElementById('ai-finding'), result = document.getElementById('ai-result');
            idle.classList.add('hidden'); result.classList.add('hidden'); finding.classList.remove('hidden');
            setTimeout(() => {
                finding.classList.add('hidden'); result.classList.remove('hidden');
                const p = (10 + val.length * 0.2).toFixed(2);
                document.getElementById('res-price').innerText = `$${p}`;
                document.getElementById('pay-total').innerText = `$${p}`;
                document.getElementById('res-time').innerText = `${Math.floor(Math.random()*40+20)} mins`;
                document.getElementById('res-reasoning').innerText = translations[currentLang]['res-reasoning'](val, selectedLocation);
                const local = brokers.filter(b => b.city === selectedLocation);
                const assigned = local.length > 0 ? local[Math.floor(Math.random()*local.length)] : brokers[0];
                document.getElementById('broker-name').innerText = assigned.name;
                document.getElementById('broker-img').src = assigned.img;
            }, 1200);
        }

        function openPayment() { document.getElementById('payment-modal').classList.remove('hidden'); }
        function closePayment() { document.getElementById('payment-modal').classList.add('hidden'); }
        function confirmPay(m) {
            alert(translations[currentLang]['alert-success'](document.getElementById('pay-total').innerText, m, document.getElementById('broker-name').innerText));
            closePayment();
        }

        // --- Physics Engine ---
        const cursor = document.getElementById('custom-cursor');
        const hatStrap = document.getElementById('hat-strap');
        let mouseX = window.innerWidth/2, mouseY = window.innerHeight/2;
        let hatX = mouseX, hatY = mouseY;
        let strapX = mouseX, strapY = mouseY + 40;
        
        const numCars = 4;
        const restDist = 105; 
        const friction = 0.92;
        const gravity = 1.0; 
        const autoSpeed = 0.8;
        
        let railYPos = 0;
        function updateRailLevel() {
            const railEl = document.getElementById('main-rail');
            const container = document.querySelector('.train-rail-container');
            if(railEl && container) {
                const rRect = railEl.getBoundingClientRect();
                const cRect = container.getBoundingClientRect();
                railYPos = rRect.top - cRect.top; 
            }
        }

        let grabbedCarIndex = -1;
        const cars = [];
        const carElements = [];

        function initCars() {
            updateRailLevel();
            for (let i = 0; i < numCars; i++) {
                const startX = -400 - (i * restDist);
                cars.push({ x: startX, y: railYPos, oldX: startX - autoSpeed, oldY: railYPos, width: i === 0 ? 110 : 95 });
                carElements.push(document.getElementById(`car-${i}`));
            }
        }

        window.addEventListener('mousemove', e => { mouseX = e.clientX; mouseY = e.clientY; });
        window.addEventListener('mousedown', e => {
            carElements.forEach((el, i) => {
                if (!el) return;
                const r = el.getBoundingClientRect();
                if (e.clientX >= r.left && e.clientX <= r.right && e.clientY >= r.top && e.clientY <= r.bottom) {
                    grabbedCarIndex = i;
                    cursor.classList.add('grabbing');
                }
            });
        });
        window.addEventListener('mouseup', () => { grabbedCarIndex = -1; cursor.classList.remove('grabbing'); });
        window.addEventListener('resize', updateRailLevel);

        function updatePhysics() {
            const container = document.querySelector('.train-rail-container');
            if(!container) return;
            const containerRect = container.getBoundingClientRect();

            for (let i = 0; i < numCars; i++) {
                let c = cars[i];
                if (i === grabbedCarIndex) {
                    c.oldX = c.x; c.oldY = c.y;
                    c.x = mouseX - containerRect.left - c.width/2; 
                    c.y = mouseY - containerRect.top - 25; 
                } else {
                    let vx = (c.x - c.oldX) * friction, vy = (c.y - c.oldY) * friction;
                    c.oldX = c.x; c.oldY = c.y;
                    c.x += vx; c.y += vy + gravity;
                }
            }

            for (let j = 0; j < 15; j++) {
                for (let i = 0; i < numCars - 1; i++) {
                    let a = cars[i], b = cars[i+1];
                    let dx = b.x - a.x, dy = b.y - a.y;
                    let dist = Math.sqrt(dx*dx + dy*dy);
                    if (dist === 0) continue;
                    let diff = (restDist - dist) / dist;
                    let offX = dx * diff * 0.5, offY = dy * diff * 0.5;
                    if (i !== grabbedCarIndex) { a.x -= offX; a.y -= offY; }
                    if (i+1 !== grabbedCarIndex) { b.x += offX; b.y += offY; }
                }
            }

            cars.forEach((c, i) => {
                if (!carElements[i]) return;
                
                if (c.y > railYPos) { 
                    c.y = railYPos; 
                    let vx = c.x - c.oldX;
                    c.oldX = c.x - (vx * 0.9);
                    if (grabbedCarIndex === -1) c.x += autoSpeed;
                }

                if (grabbedCarIndex === -1 && cars[0].x > window.innerWidth + 500) {
                    cars.forEach((cc, ii) => { cc.x = -600 - (ii*restDist); cc.oldX = cc.x - autoSpeed; cc.y = railYPos; });
                }

                let vx = c.x - c.oldX, vy = c.y - c.oldY;
                let rotation = (Math.abs(c.y - railYPos) < 1.5) ? 0 : Math.atan2(vy, vx) * (180/Math.PI);
                carElements[i].style.transform = `translate3d(${c.x}px, ${c.y-44}px, 0) rotate(${rotation}deg)`;
            });
        }

        function animate() {
            // TĂNG TỐC ĐỘ NỘI SUY: Từ 0.2 lên 0.6 để con trỏ bám sát chuột hơn
            hatX += (mouseX - hatX) * 0.6; 
            hatY += (mouseY - hatY) * 0.6;
            
            // DÂY QUAI: Tăng từ 0.1 lên 0.3 để phản ứng nhanh hơn
            strapX += (hatX - strapX) * 0.3; 
            strapY += (hatY + 40 - strapY) * 0.3;
            
            if(cursor) cursor.style.transform = `translate3d(${hatX-40}px, ${hatY-5}px, 0) rotate(${-45 + (mouseX-hatX)*0.1}deg)`;
            if(hatStrap) hatStrap.setAttribute('d', `M30 65 Q ${50 + (strapX-hatX)*1.2} ${100 + (strapY-(hatY+40))*1.2} 70 65`);
            
            updatePhysics();
            requestAnimationFrame(animate);
        }

        window.onload = () => {
            initCars();
            animate();
        };
    </script>
</body>
</html>
