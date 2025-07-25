import React, { useState, useEffect } from 'react';
import { MenuBarTrigger } from './components/MenuBarTrigger';
import { MenuBarClipboard } from './components/MenuBarClipboard';
import { ClipboardHistory } from './components/ClipboardHistory';
import { Toaster } from './components/ui/sonner';
import { Button } from './components/ui/button';
import { X } from 'lucide-react';

export default function App() {
  const [isOpen, setIsOpen] = useState(false);
  const [showFullApp, setShowFullApp] = useState(false);
  const [itemCount, setItemCount] = useState(6); // Mock count

  // Close dropdown when clicking outside
  useEffect(() => {
    const handleClickOutside = () => {
      setIsOpen(false);
    };

    if (isOpen) {
      document.addEventListener('click', handleClickOutside);
      return () => document.removeEventListener('click', handleClickOutside);
    }
  }, [isOpen]);

  // Keyboard shortcut to toggle (simulate ⌘+Shift+V)
  useEffect(() => {
    const handleKeyboard = (e: KeyboardEvent) => {
      if (e.metaKey && e.shiftKey && e.key === 'v') {
        e.preventDefault();
        setIsOpen(prev => !prev);
      }
      if (e.key === 'Escape') {
        setIsOpen(false);
        setShowFullApp(false);
      }
    };

    document.addEventListener('keydown', handleKeyboard);
    return () => document.removeEventListener('keydown', handleKeyboard);
  }, []);

  const handleOpenFullApp = () => {
    setShowFullApp(true);
    setIsOpen(false);
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 via-white to-purple-50">
      {/* Menu Bar Trigger */}
      <MenuBarTrigger 
        isOpen={isOpen} 
        onClick={() => setIsOpen(!isOpen)} 
        itemCount={itemCount}
      />
      
      {/* Menu Bar Dropdown */}
      <MenuBarClipboard 
        isOpen={isOpen} 
        onClose={() => setIsOpen(false)}
        onOpenFullApp={handleOpenFullApp}
      />
      
      {/* Background overlay when open */}
      {isOpen && (
        <div 
          className="fixed inset-0 bg-black/10 z-30 backdrop-blur-sm"
          onClick={() => setIsOpen(false)}
        />
      )}

      {/* Full App Modal */}
      {showFullApp && (
        <div className="fixed inset-0 bg-black/50 z-50 flex items-center justify-center p-4">
          <div className="relative w-full max-w-4xl h-full max-h-[90vh] bg-gradient-to-br from-blue-50 via-white to-purple-50 rounded-xl overflow-hidden">
            {/* Close button */}
            <Button
              variant="ghost"
              size="sm"
              onClick={() => setShowFullApp(false)}
              className="absolute top-4 right-4 z-10 bg-white/80 hover:bg-white/90 backdrop-blur-sm"
            >
              <X className="size-4" />
            </Button>
            
            {/* Background decorative elements */}
            <div className="absolute inset-0 overflow-hidden pointer-events-none">
              <div className="absolute top-1/4 left-1/4 w-72 h-72 bg-blue-200/20 rounded-full blur-3xl"></div>
              <div className="absolute bottom-1/4 right-1/4 w-96 h-96 bg-purple-200/20 rounded-full blur-3xl"></div>
              <div className="absolute top-3/4 left-1/2 w-64 h-64 bg-pink-200/20 rounded-full blur-3xl"></div>
            </div>
            
            {/* Full clipboard interface */}
            <div className="relative z-10 w-full h-full p-6 flex items-center justify-center">
              <ClipboardHistory />
            </div>
          </div>
        </div>
      )}

      {/* Main content area - simulating desktop */}
      <div className="pt-10 min-h-screen flex items-center justify-center">
        <div className="text-center">
          <div className="mb-8">
            <h1 className="text-3xl text-gray-800 mb-4">Clipboard Manager</h1>
            <p className="text-gray-600 mb-6 max-w-md">
              Your clipboard history is now accessible from the menu bar above. 
              Click the Clipboard button or use <kbd className="bg-gray-200 px-2 py-1 rounded text-sm">⌘⇧V</kbd> to open.
            </p>
          </div>

          <div className="bg-white/80 backdrop-blur-xl border border-gray-200/50 rounded-xl shadow-xl p-8 max-w-md">
            <h3 className="text-lg mb-4 text-gray-800">Quick Actions</h3>
            <div className="space-y-3 text-sm text-gray-600">
              <div className="flex items-center justify-between p-3 bg-gray-50/50 rounded-lg">
                <span>Open Clipboard</span>
                <kbd className="bg-gray-200 px-2 py-1 rounded text-xs">⌘⇧V</kbd>
              </div>
              <div className="flex items-center justify-between p-3 bg-gray-50/50 rounded-lg">
                <span>Copy Selection</span>
                <kbd className="bg-gray-200 px-2 py-1 rounded text-xs">⌘C</kbd>
              </div>
              <div className="flex items-center justify-between p-3 bg-gray-50/50 rounded-lg">
                <span>Paste Item</span>
                <kbd className="bg-gray-200 px-2 py-1 rounded text-xs">⌘V</kbd>
              </div>
              <div className="flex items-center justify-between p-3 bg-blue-50/50 rounded-lg border border-blue-200/30">
                <span>Open Full App</span>
                <Button 
                  variant="outline" 
                  size="sm" 
                  onClick={handleOpenFullApp}
                  className="h-6 px-3 text-xs"
                >
                  Open
                </Button>
              </div>
            </div>
          </div>
        </div>
      </div>
      
      <Toaster position="top-right" />
    </div>
  );
}