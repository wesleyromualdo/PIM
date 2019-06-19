<?php

require_once APPRAIZ . "/includes/ActiveRecord/classes/corporativo/entidades_juridica.php";

class entidades_juridicaService extends entidades_juridica {


    public function parseLegado($arrDadoPj) {

        $arrDadoLegado = $this->normalizeRetornoLegado($arrDadoPj);
        
        foreach($arrDadoLegado as $k =>$val):
            if (ctype_upper($k)){continue;}
            $return[] = "$k#{$val}";
        endforeach;

        if(is_array($return)){
            $return = implode('|', $return) ;
        }

        return $return;

    }



    public function parseJson($arrDadoPj) {
        
        $retorno =  json_encode($arrDadoPj);

        return $retorno;

    }




    /*
        realiza um de-para para adaptar a respostar para as consultas legado    
     */

    private function normalizeRetornoLegado($arrDadoPj) {


        $peparaArrMerge['nu_cep'] = ['enjendcep'];
        $peparaArrMerge['ds_localidade'] = $arrDadoPj['mundescricao'] ;
        $peparaArrMerge['ds_bairro'] = $arrDadoPj['enjendbairro'] ;
        $peparaArrMerge['ds_logradouro'] = $arrDadoPj['enjendlogradouro'] ;
        $peparaArrMerge['ds_logradouro_comp'] = $arrDadoPj['enjendcomplemento'] ;
        $peparaArrMerge['ds_numero'] = $arrDadoPj['enjendnumero'] ;
        $peparaArrMerge['no_empresarial_rf'] = $arrDadoPj['enjrazaosocial'] ;
        $peparaArrMerge['nu_cnpj_rf'] = $arrDadoPj['enjcnpj'] ;


        /* 
            @array base de resposta para o legado  
            
        */
        $retornoLegado['tp_estabelecimento_rf'] = '';
        $retornoLegado['no_empresarial_rf'] = '' ;
        $retornoLegado['no_fantasia_rf'] = '' ;
        $retornoLegado['st_cadastral_rf'] = '' ;
        $retornoLegado['dt_st_cadastral_rf'] = '' ;
        $retornoLegado['no_cidade_exterior_rf'] = '' ;
        $retornoLegado['co_codigo_pais_rf'] = '' ;
        $retornoLegado['no_pais_rf'] = '' ;
        $retornoLegado['co_natureza_juridica_rf'] = '' ;
        $retornoLegado['dt_abertura_rf'] = '' ;
        $retornoLegado['co_cnae_principal_rf'] = '' ;
        $retornoLegado['nu_cpf_responsavel_rf'] = '' ;
        $retornoLegado['no_responsavel_rf'] = '' ;
        $retornoLegado['nu_capital_social_rf'] = '' ;
        $retornoLegado['tp_crc_contador_pj_rf'] = '' ;
        $retornoLegado['nu_classificacao_crc_contador_pj_rf'] = '' ;
        $retornoLegado['nu_crc_contador_pj_rf'] = '' ;
        $retornoLegado['sg_uf_crc_contador_pj_rf'] = '' ;
        $retornoLegado['nu_cnpj_contador_rf'] = '' ;
        $retornoLegado['tp_crc_contador_pf_rf'] = '' ;
        $retornoLegado['nu_classificacao_crc_contador_pf_rf'] = '' ;
        $retornoLegado['nu_crc_contador_pf_rf'] = '' ;
        $retornoLegado['sg_uf_crc_contador_pf_rf'] = '' ;
        $retornoLegado['nu_cpf_contador_rf'] = '' ;
        $retornoLegado['ds_porte_rf'] = '' ;
        $retornoLegado['ds_opcao_simples_rf'] = '' ;
        $retornoLegado['dt_opcao_simples_rf'] = '' ;
        $retornoLegado['dt_exclusao_simples_rf'] = '' ;
        $retornoLegado['nu_cnpj_sucedida_rf'] = '' ;
        $retornoLegado['dt_cadastro'] = '' ;
        $retornoLegado['CNAESecundario'] = '' ;
        $retornoLegado['CNPJSucessora'] = '' ;
        $retornoLegado['nu_cnpj_rf'] = '' ;
        $retornoLegado['co_cidade'] = '' ;
        $retornoLegado['co_tipo_endereco_pessoa'] = '' ;
        $retornoLegado['sg_uf'] = '' ;
        $retornoLegado['ds_localidade'] = '' ;
        $retornoLegado['ds_bairro'] = '' ;
        $retornoLegado['ds_logradouro'] = '' ;
        $retornoLegado['ds_logradouro_comp'] = '' ;
        $retornoLegado['ds_numero'] = '' ;
        $retornoLegado['nu_cep'] = '' ;
        $retornoLegado['ds_ponto_referencia'] = '' ;
        $retornoLegado['ds_tipo_logradouro'] = '' ; 
        $retornoLegado['co_tipo_contato_pessoa'] = '' ;
        $retornoLegado['ds_contato_pessoa'] = '' ;
        $retornoLegado['tp_socio_rf'] = '' ;
        $retornoLegado['no_socio_rf'] = '' ;
        $retornoLegado['nu_percentual_participacao_rf'] = '' ;
        $retornoLegado['co_pais_origem_rf'] = '' ;
        $retornoLegado['no_pais_origem_rf'] = '' ;
        $retornoLegado['nu_socio_rf'] = '' ; 


        return array_merge($retornoLegado, $peparaArrMerge);


    }

}





