<?php

class Par3_Model_Aviso_conteudo extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.aviso_conteudo';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'actid',
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'actid' => null,
        'usucpf' => null,
        'acttitulo' => null,
        'actconteudo' => null,
        'actpermiteciente' => null,
        'actinicio' => null,
        'actfim' => null,
        'actcriacao' => null,
        'actstatus' => null,
        'acttipoesfera' => null
    );

    public function sqlListar()
    {
        $sql = '
            SELECT
              ac.actid,
              ac.acttitulo,
              TO_CHAR(ac.actinicio, \'DD/MM/YYYY\') || \' até \' || TO_CHAR(ac.actfim, \'DD/MM/YYYY\') AS periodo,
              u.usunome,
              ARRAY_TO_STRING(ARRAY_AGG(s.sisdiretorio||\' - \'||p.pfldsc), \';<br> \') || \'.\' AS perfil,
              CASE WHEN ac.acttipoesfera = \'M\'
                THEN \'Municipal\'
              WHEN ac.acttipoesfera = \'E\'
                THEN \'Estadual\'
              ELSE \'Todos\'
              END AS acttipoesfera,
              (
                SELECT
                  DISTINCT
                  ARRAY_TO_STRING(ARRAY_AGG((estuf)), \'; \')
                FROM
                  par3.aviso_conteudo_estado
                WHERE
                  actid = ac.actid AND acestatus = \'A\') AS estados,
              (
                SELECT
                  DISTINCT
                  ARRAY_TO_STRING(ARRAY_AGG((mundescricao)), \'; \')
                FROM
                  par3.aviso_conteudo_municipio acm INNER JOIN
                  territorios.municipio mun
                    ON mun.muncod = acm.muncod
                WHERE
                  actid = ac.actid AND acmstatus = \'A\') AS municipios,
              to_char(ac.actcriacao, \'DD/MM/YYYY\') AS actcriacao
            FROM
              par3.aviso_conteudo ac
              INNER JOIN seguranca.usuario u ON u.usucpf = ac.usucpf
              INNER JOIN par3.aviso_conteudo_perfil ap ON ap.actid = ac.actid AND ap.acpstatus = \'A\'
              INNER JOIN seguranca.perfil p ON p.pflcod = ap.pflcod
              INNER JOIN seguranca.sistema s on s.sisid = p.sisid
            WHERE
              ac.actstatus = \'A\'
            GROUP BY
              ac.actid,
              u.usunome
            ORDER BY
              ac.actcriacao desc
        ';
        return $sql;
    }

    public function carregarAvisosPorUsuario($where = array())
    {
        $arWhere = array();
        if ($where['not(actid)']) {
            $where['not(actid)'] = (array) $where['not(actid)'];
            $arWhere[] = "ac.actid NOT IN (" . (implode(",", $where['not(actid)'])) . ")";
        }

        $sqlEstadoClausula = "SELECT 1 FROM par3.aviso_conteudo_estado ace WHERE ace.actid = ac.actid and acestatus = 'A' limit 1";
        $sqlEstadoFiltro = "SELECT estuf FROM par3.aviso_conteudo_estado ace WHERE ace.actid = ac.actid and acestatus = 'A'";
        $sqlMunicipioClausula = "SELECT 1 FROM par3.aviso_conteudo_municipio acm WHERE acm.actid = ac.actid and acmstatus = 'A' limit 1";
        $sqlMunicipioFiltro = "SELECT muncod FROM par3.aviso_conteudo_municipio acm WHERE acm.actid = ac.actid and acmstatus = 'A'";
        
        if ($_SESSION['sisid'] == 23) {
            $joinResponsabilidade = "left JOIN par.usuarioresponsabilidade rpu ON rpu.rpustatus = 'A' AND 
                											                      rpu.usucpf = pu.usucpf and
                											                      rpu.pflcod = ap.pflcod";
            $selectPerfilSemResponsabilidade = "select 
                    								p.pflcod
                    							from
                    								seguranca.perfil p
                    							join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and 
                    															   pu.usucpf = '{$_SESSION['usucpf']}'
                    							where
                    								p.sisid = {$_SESSION['sisid']} and 
                    								p.pflcod not in (select pflcod from par.tprperfil)";
        } else {
            $joinResponsabilidade = "left JOIN par3.usuarioresponsabilidade rpu ON rpu.rpustatus = 'A' AND 
                											                      rpu.usucpf = pu.usucpf and
                											                      rpu.pflcod = ap.pflcod";            
            $selectPerfilSemResponsabilidade = "select 
                    								p.pflcod
                    							from
                    								seguranca.perfil p
                    							join seguranca.perfilusuario pu on pu.pflcod = p.pflcod and 
                    															   pu.usucpf = '{$_SESSION['usucpf']}'
                    							where
                    								p.sisid = {$_SESSION['sisid']} and 
                    								p.pflcod not in (select pflcod from par3.tprperfil)";
        }
        
        $sql = "select
                    distinct
                    ac.actid,
                    ac.actconteudo,
                    ac.actpermiteciente,
                    au.acuciente   
                from
                	par3.aviso_conteudo ac
                join par3.aviso_conteudo_perfil ap on ap.actid = ac.actid and 
                									  ap.acpstatus = 'A'
                join seguranca.perfil p on p.pflcod = ap.pflcod and 
                						   p.sisid = {$_SESSION['sisid']}
                join seguranca.perfilusuario pu on pu.pflcod = ap.pflcod and pu.usucpf = '{$_SESSION['usucpf']}'
                {$joinResponsabilidade}
                LEFT JOIN par3.aviso_conteudo_usuario au ON au.actid = ac.actid AND 
                											au.acustatus = 'A'
                										  	AND au.usucpf = pu.usucpf
                WHERE 
                    (au.acuciente = false OR au.acuciente IS NULL) AND 
                    ac.actstatus = 'A' AND 
                    now() BETWEEN ac.actinicio AND ac.actfim and
                    ".(count($arWhere) > 0 ? implode(" and ", $arWhere) . " AND " : "")."
                	(
                        ap.pflcod in (
                            {$selectPerfilSemResponsabilidade}
                        ) OR 
                        case 
                    		when ac.acttipoesfera = 'T' then
                    			case 
                    				-- Se não tiver filtro por ESTADO nem MUNICÍPIO pega todos
                    				when (($sqlMunicipioClausula) is null and
                    					  ($sqlEstadoClausula) is null) then
                    					true
                    				-- Se NÃO tem filtro por MUNICÍPIO pega por ESTADO e todos os MUNICÍPIOS do aludido estado.
                    				when ($sqlMunicipioClausula) is null then
                    					(rpu.estuf in ($sqlEstadoFiltro) or
                    					 rpu.muncod in (
                    						select 
                    							muncod 
                    						from 
                    							territorios.municipio 
                    						where 
                    							estuf in ($sqlEstadoFiltro)))
                    				-- Se NÃO tem filtro por ESTADO pega somente por MUNICÍPIO	
                    				when ($sqlEstadoClausula) is null then 
                    					rpu.muncod in ($sqlMunicipioFiltro)
                    				-- SE tiver filtro por ESTADO e MUNICÍPIO pega pelos dois
                    				when (($sqlEstadoClausula) is not null and
                    					  ($sqlMunicipioClausula) is not null) then
                    					(rpu.estuf in ($sqlEstadoFiltro) OR
                    					 rpu.muncod in ($sqlMunicipioFiltro))
                    			end
                    		when ac.acttipoesfera = 'E' then
                    			case 
                    				-- Se NÃO selecionar nenhum ESTADO, envia para todos
                    				when ($sqlEstadoClausula) is null then
                    					true
                    				-- envia para os ESTADOS selecionados	
                    				else
                    					rpu.estuf in ($sqlEstadoFiltro)
                    			end		
                    		when ac.acttipoesfera = 'M' then
                    			case
                                    -- Se NÃO filtrar por ESTADO nem MUNICÍPIO pega todos 
                    				when (($sqlMunicipioClausula) is null and 
                    					  ($sqlEstadoClausula) is null) then
                    					true
                                    -- Se filtrar por ESTADO, somente, pega TODOS os municípios daquele ESTADO.
                    				when ($sqlMunicipioClausula) is null then
                    					rpu.muncod in (
                    						select 
                    							muncod 
                    						from 
                    							territorios.municipio 
                    						where 
                    							estuf in ($sqlEstadoFiltro))
                    				else 
                    					rpu.muncod in ($sqlMunicipioFiltro)
                    			end
                    	end
                    )";
        
        $dado = $this->carregar($sql);
        return ($dado ? $dado : array());
    }

    public static function getEstadoMunicipio($estado = null)
    {
        $sql = "SELECT DISTINCT muncod AS codigo, estuf ||' - '|| mundescricao AS descricao FROM territorios.municipio ORDER BY descricao ASC";
        if (!is_null($estado)) {
            $sql = sprintf("SELECT DISTINCT muncod, estuf ||' - '|| mundescricao AS mundescricao FROM territorios.municipio WHERE estuf = '%s' ORDER BY mundescricao ASC", $estado);
        }

        return $sql;
    }

    public static function getEstado()
    {
        return "SELECT DISTINCT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao ASC";
    }
}