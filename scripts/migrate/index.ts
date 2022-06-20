import { APIClient, UInt64 } from "@greymass/eosio";

global.fetch = require('isomorphic-fetch');

const url = "https://eos.api.eosnation.io";
const api = new APIClient({url});

const code = "blend.gems"

interface NFT {
    collection_name: string;
    template_id: number;
    schema_name?: string;
}

interface ExtendedAsset {
    contract: string;
    quantity: string;
}

interface Blends {
    id: NFT,
    recipe_ids: number[],
    description: string;
    plugin: string;
    quantity: null | ExtendedAsset;
    start_time: null | string;
    end_time: null | string;
}

interface Recipes {
    id: number;
    templates: NFT[];
}

async function get_collections() {
    const { rows } = await api.v1.chain.get_table_rows({code, scope: code, table: "collections"});
    return rows[0].collection_names;
}

async function get_blends(collection_name: string): Promise<Blends[]> {
    const { rows } = await api.v1.chain.get_table_rows({code, scope: collection_name, table: "blends"});
    return rows;
}

async function get_recipe(collection_name: string, recipe_id: number ): Promise<Recipes> {
    const { rows } = await api.v1.chain.get_table_rows({code, scope: collection_name, table: "recipes", lower_bound: UInt64.from(recipe_id), upper_bound: UInt64.from(recipe_id) });
    return rows[0];
}

(async () => {
    const collection_names = await get_collections();
    for ( const collection_name of collection_names) {
        console.log('\n# collection_name:', collection_name);

        console.log(`cleos -u ${url} push action ${code} reset '[blends, ${collection_name}]' -p ${code}`);
        console.log(`cleos -u ${url} push action ${code} reset '[recipes, ${collection_name}]' -p ${code}`);

        const blends = await get_blends( collection_name );
        for ( const blend of blends ) {
            console.log('\n# blends:', blend.id.collection_name, blend.id.template_id);
            let start_time = blend.start_time ? `"${blend.start_time}"` : "null";
            let end_time = blend.end_time ? `"${blend.end_time}"` : "null";
            if ( start_time == `"1970-01-01T00:00:00"`) start_time = "null";
            if ( end_time == `"1970-01-01T00:00:00"`) end_time = "null";

            const plugin = blend.plugin ? `${blend.plugin}` : "null";
            console.log(`cleos -u ${url} push action ${code} setblend '[${collection_name}, ${blend.id.template_id}, "${blend.description}", ${plugin}, null, ${start_time}, ${end_time}]' -p ${code}`);

            console.log('\n# recipes:', blend.id.collection_name, blend.id.template_id);
            for ( const recipe_id of blend.recipe_ids ) {
                const recipe = await get_recipe(collection_name, recipe_id);
                // console.log(recipe.templates)
                const templates = recipe.templates.map(row => { return `[${row.collection_name},${row.template_id}]` })
                console.log(`cleos -u ${url} push action ${code} addrecipe '[${collection_name}, ${blend.id.template_id}, [${templates}]]' -p ${code}`);
            }
        }
    }
})()
