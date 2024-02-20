declare module 'font-info' {
    export interface FontDescriptor {
        readonly path: string;
        readonly family: string;
        readonly style: string;
    }

    /**
     * Get a list of available fonts from the system catalog synchronously.
     * @returns All font descriptors available on the system.
     */
    export function getAvailableFontsSync(): FontDescriptor[];

    /**
     * Returns through a callback all fonts descriptors available on the system synchronously
     * 
     * @param callback A callback function
     */
    export function getAvailableFontsSync(callback: (fonts: FontDescriptor[]) => void): void;
    
    /**
     * Get a list of available fonts from the system catalog synchronously.
     * @returns A promise to all font descriptors available on the system.
     */
    export function getAvailableFonts(): Promise<FontDescriptor[]>;
}