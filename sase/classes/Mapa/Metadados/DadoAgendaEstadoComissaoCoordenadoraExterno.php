<?php
class DadoAgendaEstadoComissaoCoordenadoraExterno extends DadosAbstract implements DadosInterface, DadosLegendaInterface
{
    /**
     * Carrega metadados do Mapa Agenda de trabalho da comissão coordenadora - estado
     *
     * @return array
     */
    public function carregaDado()
    {
        global $db;
        if (count($this->estuf) > 0) {
            $and = " AND e.estuf IN ('".(implode("','",$this->estuf))."')";
        }
        $sql = <<<DML
                SELECT  DISTINCT
                        CASE WHEN acedatainclusao IS NOT NULL
                            THEN '#006600'
                            ELSE '#FFFFFF'
                        END AS cor,
            			CASE WHEN acedatainclusao IS NOT NULL
                            THEN 'Preenchido'
                            ELSE 'Não preenchido'
                        END AS situacao,
                        e.estuf
                FROM territorios.estado AS e
                LEFT JOIN sase.agendacomissaocoordestado AS a ON a.estuf = e.estuf
                WHERE 1=1
                {$and}
DML;
        $this->dado = $db->carregar($sql);
    }

    public function dadosDaLegenda()
    {
        global $db;

        $estuf =$this->estuf;

        $where = ( ( $estuf!='' && count($estuf)>0 ) ? " and m.estuf in ( '". ( implode( "','", $estuf ) ) ."' ) " : "" );

        $sql = <<<DML
            SELECT 
                count(DISTINCT e.*) as total,
                'Preenchido' as descricao,
                '#006600' as cor
            FROM 
                territorios.estado AS e 
            LEFT JOIN sase.agendacomissaocoordestado AS a ON a.estuf = e.estuf
            WHERE acedatainclusao IS NOT NULL
            {$where}
            UNION ALL
            SELECT count(DISTINCT e.*) as total,
                 'Não preenchido' as descricao,
                 '#FFFFFF' as cor
            FROM territorios.estado AS e
            LEFT JOIN sase.agendacomissaocoordestado AS a ON a.estuf = e.estuf
            WHERE acedatainclusao IS NULL
            {$where}
            ORDER BY descricao
DML;
                
        $legenda = $db->carregar($sql);
        return $legenda?$legenda:[];
    }
}
