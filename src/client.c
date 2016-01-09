extern crate user32;
extern crate kernel32;

#[allow(deprecated)]
use std::io::prelude::*;
use std::net::TcpStream;
use std::ptr;

const ERROR_ACCESS_DENIED: u32 = 0x5;
const ERROR_CLIPBOARD_NOT_OPEN: u32 = 0x58A;

struct Clipboard;

impl Clipboard {
    fn open() -> Result<Clipboard, WinApiError> {
        unsafe {
            let _open = user32::OpenClipboard(ptr::null_mut());
            try!(WinApiError::from_global());
            return Ok(Clipboard)
        }
    }
    fn get_data(&self) -> Result<String, WinApiError> {
        let value: String;
        unsafe {
            let var = user32::GetClipboardData(13 as u32);
            try!(WinApiError::from_global());
            let data = kernel32::GlobalLock(var) as *mut u16;
            let len = rust_strlen16(data);
            let raws = std::slice::from_raw_parts(data, len);
            value = String::from_utf16_lossy(raws);
            kernel32::GlobalUnlock(var);
        }
        Ok(value)
    }
}

impl Drop for Clipboard {
    fn drop(&mut self) {
        unsafe {user32::CloseClipboard();}
    }
}
#[derive(Debug, Copy, Clone)]
struct WinApiError(u32);                //help from shepmaster

impl WinApiError {
    fn from_global() -> Result<(), WinApiError> {
        let val = unsafe { kernel32::GetLastError() };
        if val != 0 {
            Err(WinApiError(val))
        } else {
            Ok(())
        }
    }
}
fn main() {
    let mut current = String::new();
    loop {
        let clip = match Clipboard::open() {
            Err(WinApiError(ERROR_ACCESS_DENIED)) |
            Err(WinApiError(ERROR_CLIPBOARD_NOT_OPEN)) | Err(WinApiError(_)) => {
                println!("{:?}", WinApiError::from_global());
                continue;
            },
            Ok(c) => c,
        };

        if current != clip.get_data().unwrap() {
            current = clip.get_data().unwrap();
            let mut client = TcpStream::connect("127.0.0.1:8080").unwrap();
            let message = clip.get_data().expect("Could not get Data!");
            println!("{}", message);
            let _ = client.write(message.as_bytes());
            println!("Sent!");
        }
        std::thread::sleep_ms(250);
    }
}


#[inline(always)]                                       // used from clipboard-win, not mine
unsafe fn rust_strlen16(buff_p: *mut u16) -> usize {
    let mut i: isize = 0;
    while *buff_p.offset(i) != 0 {
        i += 1;
    }
    return i as usize
}
