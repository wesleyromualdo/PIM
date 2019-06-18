<?php

class Par3_Model_Siope extends Modelo
{
	public function listarReceitas($dados)
	{
		if($dados['muncod']) {
			$sql = "SELECT an_receita, sg_uf, sg_uf as no_uf, co_municipio_ibge, no_municipio, vl_orig_fundeb,
       				vl_imp_mun_proprio, vl_outra_receita, vl_sal_educ, vl_outra_rec_dest_educ,
       				dt_ult_carga
			FROM integracao.rec_educ_municipio
			WHERE an_receita IS NOT NULL AND co_municipio_ibge = '{$dados['muncod']}'
			ORDER BY an_receita";
		}else{
			$sql = "SELECT an_receita, sg_uf, sg_uf as no_uf, vl_orig_fundeb, vl_imp_est_proprio,
       				vl_outra_receita, vl_sal_educ, vl_outra_rec_dest_educ, dt_ult_carga
			FROM integracao.rec_educ_estado
			WHERE an_receita IS NOT NULL AND sg_uf = '{$dados['estuf']}'
			ORDER BY an_receita";
		}

		return adapterConnection::coorporativo()->carregar($sql);
	}

	public function listarDespesas($dados)
	{
		if($dados['muncod']) {
			$sql = "SELECT an_despesa, sg_uf, co_municipio_ibge, no_municipio, ds_etapa, no_creche_pre_escola, ds_profissionais, sum(vl_despesa_empenhada) as vl_despesa_empenhada
					FROM integracao.desp_educ_mun_etapa
					WHERE an_despesa IS NOT NULL AND ds_etapa is not null and co_municipio_ibge = '{$dados['muncod']}'
					group by an_despesa, sg_uf, co_municipio_ibge, no_municipio, ds_etapa, no_creche_pre_escola, ds_profissionais
					ORDER BY 1,5,6,7";
		}else{
			$sql = "SELECT an_despesa, sg_uf, ds_etapa, no_creche_pre_escola, ds_profissionais, sum(vl_despesa_empenhada) as vl_despesa_empenhada
					FROM integracao.desp_educ_est_etapa
					WHERE an_despesa IS NOT NULL AND ds_etapa is not null and sg_uf = '{$dados['estuf']}'
					group by an_despesa, sg_uf, ds_etapa, no_creche_pre_escola, ds_profissionais
					ORDER BY 1,3,4,5";
		}

		return adapterConnection::coorporativo()->carregar($sql);
	}

	public function totalDespesa($dados)
	{
		if($dados['muncod']) {
			$sql = "SELECT an_despesa, sum(vl_despesa_empenhada) as vl_despesa_empenhada
					FROM integracao.desp_educ_mun_etapa
					WHERE ds_etapa is not null and co_municipio_ibge = '{$dados['muncod']}'
					GROUP BY an_despesa
					ORDER BY 1";
		}else{
			$sql = "SELECT an_despesa, sum(vl_despesa_empenhada) as vl_despesa_empenhada
					FROM integracao.desp_educ_est_etapa
					WHERE ds_etapa is not null and sg_uf = '{$dados['estuf']}'
					GROUP BY an_despesa
					ORDER BY 1";
		}

		return adapterConnection::coorporativo()->carregar($sql);
	}

	public function listarCumprimentos($dados,  $ano = '')
	{
		if( !empty($dados['inuid']) ){
			$instrumentoUnidade = new Par3_Model_InstrumentoUnidade( $dados['inuid'] );
			$estuf = $instrumentoUnidade->estuf;
			$muncod = $instrumentoUnidade->muncod;
			$itrid = $instrumentoUnidade->itrid;
				
			$dados['estuf'] = $estuf;
			$dados['muncod'] = $muncod;
		}
		
		$filtro = '';
		if( !empty($ano) ){
		    $filtro = " and an_exercicio_mde = '$ano' ";
		}
		
		if($dados['muncod']) {
			$sql = "SELECT an_exercicio_mde, sg_uf_mde, sg_uf_mde as no_uf_mde, co_municipio_ibge_rpm, no_municipio_rpm,
				   vl_apl_rec_mde, vl_perc_aplic_med, vl_apl_rem__prof_mag_rpm, vl_perc_aplic_mag_rpm,
				   vl_apl_exc_rem_mag_erm, vl_perc_aplic_exc_mag_erm, dt_ult_carga
			FROM integracao.cump_vinc_municipal
			WHERE an_exercicio_mde IS NOT NULL AND co_municipio_ibge_rpm = '{$dados['muncod']}' $filtro
			ORDER BY an_exercicio_mde";
		}else{
			$sql = "SELECT an_exercicio_mde, sg_uf_mde, sg_uf_mde as no_uf_mde, vl_apl_rec_mde, vl_perc_aplic_med,
				   vl_apl_rem__prof_mag_rpm, vl_perc_aplic_mag_rpm, vl_apl_exc_rem_mag_erm,
				   vl_perc_aplic_exc_mag_erm, dt_ult_carga
			FROM integracao.cump_vinc_estado
			WHERE an_exercicio_mde IS NOT NULL AND sg_uf_mde = '{$dados['estuf']}' $filtro
			ORDER BY an_exercicio_mde";
		}
        
		return adapterConnection::coorporativo()->carregar($sql);
	}

    public function transmissaoSiope($dados)
    {
        if( !empty($dados['inuid']) ){
            $instrumentoUnidade = new Par3_Model_InstrumentoUnidade( $dados['inuid'] );
            $estuf = $instrumentoUnidade->estuf;
            $muncod = $instrumentoUnidade->muncod;
            $itrid = $instrumentoUnidade->itrid;

            $dados['estuf'] = $estuf;
            $dados['muncod'] = $muncod;
        }

        if(!empty($dados['muncod'])) {
            $sql = "SELECT cod_situacao, situacao_desc FROM integracao.situacao_siope_municipal WHERE codigo_mun_ibge = '{$dados['muncod']}'";
        }else{
            $sql = "SELECT cod_situacao, situacao_desc FROM integracao.situacao_siope_estadual WHERE uf = '{$dados['estuf']}'";
        }

        return adapterConnection::coorporativo()->carregar($sql);
    }

}